// include/beman/execution26/detail/then.hpp                          -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef INCLUDED_BEMAN_EXECUTION26_DETAIL_THEN
#define INCLUDED_BEMAN_EXECUTION26_DETAIL_THEN

#include <beman/execution26/detail/completion_signatures.hpp>
#include <beman/execution26/detail/completion_signatures_for.hpp>
#include <beman/execution26/detail/default_impls.hpp>
#include <beman/execution26/detail/get_domain_early.hpp>
#include <beman/execution26/detail/impls_for.hpp>
#include <beman/execution26/detail/make_sender.hpp>
#include <beman/execution26/detail/movable_value.hpp>
#include <beman/execution26/detail/sender.hpp>
#include <beman/execution26/detail/sender_adaptor.hpp>
#include <beman/execution26/detail/sender_adaptor_closure.hpp>
#include <beman/execution26/detail/set_error.hpp>
#include <beman/execution26/detail/set_stopped.hpp>
#include <beman/execution26/detail/set_value.hpp>
#include <beman/execution26/detail/transform_sender.hpp>
#include <concepts>
#include <exception>
#include <functional>
#include <utility>

// ----------------------------------------------------------------------------

namespace beman::execution26::detail
{
    template <typename Completion>
    struct then_t
        : ::beman::execution26::sender_adaptor_closure<then_t<Completion>>
    {
        template <::beman::execution26::detail::movable_value Fun>
        auto operator()(Fun&& fun) const
        {
            return ::beman::execution26::detail::sender_adaptor{
                *this, ::std::forward<Fun>(fun)
            };
        }
        template <::beman::execution26::sender Sender,
                  ::beman::execution26::detail::movable_value Fun>
        auto operator()(Sender&& sender, Fun&& fun) const
        {
            auto domain{::beman::execution26::detail::get_domain_early(sender)};
            return ::beman::execution26::transform_sender(
                domain,
                ::beman::execution26::detail::make_sender(
                    *this,
                    ::std::forward<Fun>(fun),
                    ::std::forward<Sender>(sender)
                )
            );
        }
    };

    template <typename Completion>
    struct impls_for<then_t<Completion>>
        : ::beman::execution26::detail::default_impls
    {
        static constexpr auto complete
            = []<typename Tag, typename... Args>
                (auto, auto& fun, auto& receiver, Tag, Args&&... args)
                noexcept
                -> void
            {
                if constexpr (::std::same_as<Completion, Tag>)
                {
                    try
                    {
                        auto invoke = [&]{
                            return ::std::invoke(::std::move(fun),
                                                 ::std::forward<Args>(args)...);
                        };
                        if constexpr (::std::same_as<void, decltype(invoke())>)
                        {
                            invoke();
                            ::beman::execution26::set_value(
                                ::std::move(receiver)
                            );
                        }
                        else
                        {
                            ::beman::execution26::set_value(
                                ::std::move(receiver),
                                invoke()
                            );
                        }
                    }
                    catch (...)
                    {
                        ::beman::execution26::set_error(
                            ::std::move(receiver),
                            ::std::current_exception()
                        );
                    }
                }
                else
                {
                    Tag()(::std::move(receiver), ::std::forward<Args>(args)...);
                }
            };
    };


    template <typename Fun, typename Sender, typename Env>
    struct completion_signatures_for_impl<
        ::beman::execution26::detail::basic_sender<
            ::beman::execution26::detail::then_t<::beman::execution26::set_stopped_t>,
            Fun,
            Sender
            >,
        Env
        >
    {
        using type = ::beman::execution26::completion_signatures<
        >;
    };
}

namespace beman::execution26
{
    using then_t = ::beman::execution26::detail::then_t<::beman::execution26::set_value_t>;
    using upon_error_t = ::beman::execution26::detail::then_t<::beman::execution26::set_error_t>;
    using upon_stopped_t = ::beman::execution26::detail::then_t<::beman::execution26::set_stopped_t>;

    inline constexpr ::beman::execution26::then_t then{};
    inline constexpr ::beman::execution26::upon_error_t upon_error{};
    inline constexpr ::beman::execution26::upon_stopped_t upon_stopped{};
}

// ----------------------------------------------------------------------------

#endif
