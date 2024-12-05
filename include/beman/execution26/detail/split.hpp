// include/beman/execution26/detail/split.hpp                         -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef INCLUDED_BEMAN_EXECUTION26_DETAIL_SPLIT
#define INCLUDED_BEMAN_EXECUTION26_DETAIL_SPLIT

#include <beman/execution26/detail/atomic_intrusive_stack.hpp>
#include <beman/execution26/detail/connect_result_t.hpp>
#include <beman/execution26/detail/default_impls.hpp>
#include <beman/execution26/detail/emplace_from.hpp>
#include <beman/execution26/detail/error_types_of_t.hpp>
#include <beman/execution26/detail/completion_signatures_for.hpp>
#include <beman/execution26/detail/get_domain_early.hpp>
#include <beman/execution26/detail/get_stop_token.hpp>
#include <beman/execution26/detail/child_type.hpp>
#include <beman/execution26/detail/completion_signatures_of_t.hpp>
#include <beman/execution26/detail/sender_for.hpp>
#include <beman/execution26/detail/impls_for.hpp>
#include <beman/execution26/detail/inplace_stop_source.hpp>
#include <beman/execution26/detail/make_sender.hpp>
#include <beman/execution26/detail/type_list.hpp>
#include <beman/execution26/detail/meta_combine.hpp>
#include <beman/execution26/detail/meta_unique.hpp>
#include <beman/execution26/detail/receiver.hpp>
#include <beman/execution26/detail/stop_callback_for_t.hpp>
#include <beman/execution26/detail/stop_token_of_t.hpp>
#include <beman/execution26/detail/stoppable_token.hpp>
#include <beman/execution26/detail/value_types_of_t.hpp>

#include <atomic>
#include <optional>
#include <variant>
#include <tuple>

// ----------------------------------------------------------------------------

namespace beman::execution26::detail {

struct split_impl_t {};
template <>
struct impls_for<split_impl_t> : ::beman::execution26::detail::default_impls {

    template <class Sndr>
    struct shared_state;

    struct split_env {
        ::beman::execution26::inplace_stop_source* stop_source;

        ::beman::execution26::inplace_stop_token query(::beman::execution26::get_stop_token_t) const noexcept {
            return stop_source->get_token();
        }
    };

    struct local_state_base {
        local_state_base* next{nullptr};
        virtual auto      notify() noexcept -> void = 0;

        local_state_base() noexcept                                  = default;
        local_state_base(const local_state_base&)                    = delete;
        local_state_base(local_state_base&&)                         = delete;
        auto operator=(const local_state_base&) -> local_state_base& = delete;
        auto operator=(local_state_base&&) -> local_state_base&      = delete;

      protected:
        ~local_state_base() = default;
    };

    template <class Sndr>
    struct split_receiver {
        using receiver_concept = ::beman::execution26::receiver_t;

        explicit split_receiver(shared_state<Sndr>* state) noexcept : sh_state(state) {
            if (sh_state) {
                sh_state->inc_ref();
            }
        }

        ~split_receiver() noexcept {
            if (sh_state) {
                sh_state->dec_ref();
            }
        }

        split_receiver(split_receiver&& other) noexcept : sh_state(::std::exchange(other.sh_state, nullptr)) {}
        split_receiver& operator=(split_receiver&& other) noexcept {
            sh_state = ::std::exchange(other.sh_state, nullptr);
            return *this;
        }

        split_receiver(const split_receiver&)            = delete;
        split_receiver& operator=(const split_receiver&) = delete;

        template <class Tag, class... Args>
        void complete(Tag, Args&&... args) noexcept {
            using tuple_t = ::beman::execution26::detail::decayed_tuple<Tag, Args...>;
            try {
                sh_state->result.template emplace<tuple_t>(Tag(), ::std::forward<Args>(args)...);
            } catch (...) {
                using tuple_err = ::std::tuple<::beman::execution26::set_error_t, ::std::exception_ptr>;
                sh_state->result.template emplace<tuple_err>(::beman::execution26::set_error,
                                                             ::std::current_exception());
            }
            sh_state->notify();
        }

        template <class... Args>
        void set_value(Args&&... args) && noexcept {
            complete(::beman::execution26::set_value, ::std::forward<Args>(args)...);
        }

        template <class Error>
        void set_error(Error&& err) && noexcept {
            complete(::beman::execution26::set_error, ::std::forward<Error>(err));
        }

        void set_stopped() && noexcept { complete(::beman::execution26::set_stopped); }

        split_env get_env() const noexcept { return split_env{&sh_state->stop_src}; }

        shared_state<Sndr>* sh_state;
    };

    // [exec.split-10]
    template <class Sndr>
    struct shared_state {
        template <class... Args>
        using value_tuple = ::std::tuple<::beman::execution26::set_value_t, ::std::decay_t<Args>...>;

        template <class... Args>
        using error_tuples = ::std::variant<::std::tuple<::beman::execution26::set_error_t, ::std::decay_t<Args>>...>;

        using variant_type = ::beman::execution26::detail::meta::unique<::beman::execution26::detail::meta::combine<
            ::std::variant<::std::monostate>,
            ::std::variant<::std::tuple<::beman::execution26::set_stopped_t>>,
            ::std::variant<::std::tuple<::beman::execution26::set_error_t, ::std::exception_ptr>>,
            ::beman::execution26::error_types_of_t<Sndr, split_env, error_tuples>,
            ::beman::execution26::value_types_of_t<Sndr, split_env, value_tuple>>>;

        using state_list_type = ::beman::execution26::detail::atomic_intrusive_stack<&local_state_base::next>;

        using child_operation_state = ::beman::execution26::connect_result_t<Sndr, split_receiver<Sndr>>;

        explicit shared_state(Sndr&& sndr) {
            try {
                op_state.emplace(::beman::execution26::detail::emplace_from{[&] {
                    return ::beman::execution26::connect(::std::forward<Sndr>(sndr), split_receiver<Sndr>{this});
                }});
            } catch (...) {
                using error_tuple_t = ::std::tuple<::beman::execution26::set_error_t, ::std::exception_ptr>;
                result.template emplace<error_tuple_t>(::beman::execution26::set_error, ::std::current_exception());
                [[maybe_unused]] auto queue = waiting_states.pop_all_and_shutdown();
                assert(queue.empty());
            }
        }

        // We use an intrusive list to store the listeners that are waiting for the operation to complete.
        // if the intrusive list is empty, we start the operation
        // if the intrusive list is not empty, we push the listener to the intrusive list
        // if the intrusive list is shutdown, we immediately notify the listener
        void add_listener(local_state_base* listener) noexcept {
            // try to push the listener to the intrusive list, if the intrusive list is empty and not shutdown, start
            // the operation
            if (auto maybe_ptr = waiting_states.try_push(listener); !maybe_ptr) {
                // the queue is shutdown, immediately notify the listener
                listener->notify();
            } else if (!(*maybe_ptr)) {
                // the operation was not started yet, we are first, and we start it
                assert(op_state);
                ::beman::execution26::start(*op_state);
            }
        }

        void notify() noexcept {
            // note: this is different from stdexec.
            // we discussed lifetime durations of operation at LEWG and haven't decided yet
            // whether we should keep the operation alive as long as possible
            op_state.reset();
            auto listeners = waiting_states.pop_all_and_shutdown();
            while (auto listener = listeners.pop()) {
                listener->notify();
            }
        }

        void inc_ref() noexcept { ref_count.fetch_add(1); }

        // This is the most complicated part of the split implementation.
        // On construction, the operation state increments the ref count.
        // Before the operation is started, at least one listener is added to the queue.
        // If the ref count is decreased to one and the there are no listeners in the queue
        // the operation state is the last object holding the shared state and we can safely
        // destroy it
        //
        // it is not thread safe to destroy a split-sender and copy it at the same time
        // this is similar to how a shared_ptr is not thread safe to copy and destroy at the same time
        void dec_ref() noexcept {
            std::size_t count = ref_count.load();
            if (count == 2 && waiting_states.empty_and_not_shutdown()) {
                assert(op_state);
                [[maybe_unused]] auto listeners = waiting_states.pop_all_and_shutdown();
                assert(listeners.empty());
                op_state.reset();
            }
            if (ref_count.fetch_sub(1) == 1) {
                delete this;
            }
        }

        ::beman::execution26::inplace_stop_source stop_src{};
        variant_type                              result{};
        state_list_type                           waiting_states{};
        ::std::atomic<::std::size_t>              ref_count{0};
        ::std::optional<child_operation_state>    op_state{};
    };

    template <class Sndr, class Receiver>
    struct local_state final : local_state_base {
        using stop_token_type = ::beman::execution26::stop_token_of_t<::beman::execution26::env_of_t<Receiver>>;

        struct on_stop_type {
            shared_state<Sndr>* sh_state;
            void                operator()() noexcept { sh_state->stop_src.request_stop(); }
        };

        using on_stop_callback = ::beman::execution26::stop_callback_for_t<stop_token_type, on_stop_type>;

        explicit local_state(shared_state<Sndr>* state, Receiver& rcvr) noexcept
            : sh_state(state), receiver{std::addressof(rcvr)} {
            sh_state->inc_ref();
        }

        ~local_state() noexcept { sh_state->dec_ref(); }

        local_state(const local_state&)            = delete;
        local_state& operator=(const local_state&) = delete;
        local_state(local_state&&)                 = delete;
        local_state& operator=(local_state&&)      = delete;

        auto notify() noexcept -> void override {
            on_stop.reset();
            auto stop_token = ::beman::execution26::get_stop_token(::beman::execution26::get_env(receiver));
            if (stop_token.stop_requested()) {
                ::beman::execution26::set_stopped(std::move(*receiver));
            } else {
                assert(sh_state->result.index() > 0);
                assert(!sh_state->result.valueless_by_exception());
                try {
                    ::std::visit(
                        [&]<class Arg>(const Arg& arg) noexcept -> void {
                            if constexpr (not ::std::same_as<::std::decay_t<Arg>, ::std::monostate>) {
                                ::std::apply(
                                    [&](auto tag, const auto&... args) noexcept -> void {
                                        tag(::std::move(*receiver), args...);
                                    },
                                    arg);
                            }
                        },
                        sh_state->result);
                } catch (...) {
                    // required by clang-tidy although it is not necessary here
                    // see valueless_by_exception() check above
                    std::terminate();
                }
            }
        }

        void start() noexcept {
            on_stop.emplace(::beman::execution26::get_stop_token(::beman::execution26::get_env(*receiver)),
                            on_stop_type{sh_state});
            sh_state->add_listener(this);
        }

        std::optional<on_stop_callback> on_stop;
        shared_state<Sndr>*             sh_state;
        Receiver*                       receiver;
    };

    static constexpr auto get_state = []<typename Sender, typename Receiver>(Sender&&  sender,
                                                                             Receiver& receiver) noexcept {
        auto&& wrapper = sender.template get<1>();
        return local_state(wrapper.sh_state, receiver);
    };

    static constexpr auto start = []<class Sndr, class Rcvr>(local_state<Sndr, Rcvr>& state, Rcvr&) noexcept {
        state.start();
    };
};

template <class Sndr>
struct shared_wrapper {
    explicit shared_wrapper(impls_for<split_impl_t>::shared_state<Sndr>* state) noexcept : sh_state(state) {
        if (sh_state) {
            sh_state->inc_ref();
        }
    }

    ~shared_wrapper() noexcept {
        // NOLINTBEGIN(clang-analyzer-cplusplus.NewDelete)
        if (sh_state) {
            sh_state->dec_ref();
        }
        // NOLINTEND(clang-analyzer-cplusplus.NewDelete)
    }

    shared_wrapper(const shared_wrapper& other) noexcept : sh_state(other.sh_state) {
        if (sh_state) {
            sh_state->inc_ref();
        }
    }

    shared_wrapper(shared_wrapper&& other) noexcept : sh_state(::std::exchange(other.sh_state, nullptr)) {}

    shared_wrapper& operator=(const shared_wrapper& other) noexcept {
        // check for self-assignment was required by clang-tidy
        // although it is not necessary here
        if (this == &other) {
            return *this;
        }
        auto tmp = other;
        ::std::swap(sh_state, tmp.sh_state);
        return *this;
    }

    shared_wrapper& operator=(shared_wrapper&& other) noexcept {
        auto tmp = ::std::move(other);
        ::std::swap(sh_state, tmp.sh_state);
        return *this;
    }

    impls_for<split_impl_t>::shared_state<Sndr>* sh_state;
};

struct split_t {
    template <class Sndr>
    auto transform_sender(Sndr&& sndr) const {
        auto&& child       = ::std::forward<Sndr>(sndr).template get<2>();
        using child_type   = decltype(child);
        using shared_state = ::beman::execution26::detail::impls_for<split_impl_t>::shared_state<child_type>;
        auto* sh_state     = new shared_state{::beman::execution26::detail::forward_like<Sndr>(child)};
        return ::beman::execution26::detail::make_sender(split_impl_t{}, shared_wrapper<child_type>{sh_state});
    }

    template <class Sender>
        requires beman::execution26::sender_in<Sender, impls_for<split_impl_t>::split_env>
    auto operator()(Sender&& sender) const {
        auto domain{::beman::execution26::detail::get_domain_early(sender)};
        return ::beman::execution26::transform_sender(
            domain, ::beman::execution26::detail::make_sender(*this, {}, ::std::forward<Sender>(sender)));
    }
};

template <class Sndr, class Env>
struct completion_signatures_for_impl<
    ::beman::execution26::detail::basic_sender<::beman::execution26::detail::split_impl_t,
                                               ::beman::execution26::detail::shared_wrapper<Sndr>>,
    Env> {
    template <class... Args>
    using make_value_completions =
        ::beman::execution26::completion_signatures<::beman::execution26::set_value_t(const std::decay_t<Args>&...)>;

    template <class... Args>
    using make_error_completions =
        ::beman::execution26::completion_signatures<::beman::execution26::set_error_t(const std::decay_t<Args>&)...>;

    using value_completions = ::beman::execution26::
        value_types_of_t<Sndr, Env, make_value_completions, ::beman::execution26::detail::meta::combine>;

    using error_completions = ::beman::execution26::error_types_of_t<Sndr, Env, make_error_completions>;

    using fixed_completions =
        ::beman::execution26::completion_signatures<::beman::execution26::set_stopped_t(),
                                                    ::beman::execution26::set_error_t(std::exception_ptr)>;

    using type = ::beman::execution26::detail::meta::unique<
        ::beman::execution26::detail::meta::combine<fixed_completions, value_completions, error_completions>>;
};

} // namespace beman::execution26::detail

namespace beman::execution26 {
using split_t = ::beman::execution26::detail::split_t;

inline constexpr ::beman::execution26::split_t split{};
} // namespace beman::execution26

#endif
