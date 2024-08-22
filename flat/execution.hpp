// include/Beman/Execution26/execution.hpp -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef INCLUDED_BEMAN_EXECUTION26_EXECUTION
#define INCLUDED_BEMAN_EXECUTION26_EXECUTION

#include <utility>
#include <concepts>
#include <type_traits>
#include <atomic>
#include <memory>
#include <mutex>
#include <thread>
#include <cstddef>
#include <cassert>
#include <exception>
#include <system_error>
// include/Beman/Execution26/detail/queryable.hpp                     -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef INCLUDED_BEMAN_EXECUTION26_DETAIL_QUERYABLE
#define INCLUDED_BEMAN_EXECUTION26_DETAIL_QUERYABLE


// ----------------------------------------------------------------------------

namespace Beman::Execution26::Detail
{
    template <typename T>
    concept queryable = ::std::destructible<T>;
}

// ----------------------------------------------------------------------------

#endif
// include/Beman/Execution26/detail/forwarding_query.hpp              -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef INCLUDED_BEMAN_EXECUTION26_DETAIL_FORWARDING_QUERY
#define INCLUDED_BEMAN_EXECUTION26_DETAIL_FORWARDING_QUERY


// ----------------------------------------------------------------------------

namespace Beman::Execution26
{
    struct forwarding_query_t
    {
        template <typename Object>
            requires requires(Object&& object, forwarding_query_t const& query)
            {
                 { ::std::forward<Object>(object).query(query) } noexcept -> ::std::same_as<bool>;
            }
        constexpr auto operator()(Object&& object) const noexcept -> bool
        {
            return ::std::forward<Object>(object).query(*this);
        }
        template <typename Object>
        constexpr auto operator()(Object&&) const noexcept -> bool
        {
            return ::std::derived_from<Object, ::Beman::Execution26::forwarding_query_t>;
        }
    };

    inline constexpr forwarding_query_t forwarding_query{};
}

// ----------------------------------------------------------------------------

#endif
// include/Beman/Execution26/detail/movable_value.hpp                 -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef INCLUDED_BEMAN_EXECUTION26_DETAIL_MOVABLE_VALUE
#define INCLUDED_BEMAN_EXECUTION26_DETAIL_MOVABLE_VALUE


// ----------------------------------------------------------------------------

namespace Beman::Execution26::Detail
{
    template <typename T>
    concept movable_value
        =  ::std::move_constructible<::std::decay_t<T>>
        && ::std::constructible_from<::std::decay_t<T>, T>
        && (not ::std::is_array_v<::std::remove_reference_t<T>>)
        ;
}

// ----------------------------------------------------------------------------

#endif
// include/Beman/Execution26/detail/matching_sig.hpp                  -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef INCLUDED_BEMAN_EXECUTION26_DETAIL_MATCHING_SIG
#define INCLUDED_BEMAN_EXECUTION26_DETAIL_MATCHING_SIG


// ----------------------------------------------------------------------------

namespace Beman::Execution26::Detail
{
    template <typename Fun>
    struct matching_sig_transform
    {
        using type = Fun;
    };
    template <typename Return, typename... Args>
    struct matching_sig_transform<Return(Args...)>
    {
        using type = Return(Args&&...);
    };

    template <typename Fun1, typename Fun2>
    inline constexpr bool matching_sig
        = ::std::same_as<
            typename ::Beman::Execution26::Detail::matching_sig_transform<Fun1>::type,
            typename ::Beman::Execution26::Detail::matching_sig_transform<Fun2>::type
            >
        ;
}

// ----------------------------------------------------------------------------

#endif
// include/Beman/Execution26/detail/as_except_ptr.hpp                 -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef INCLUDED_BEMAN_EXECUTION26_DETAIL_AS_EXCEPT_PTR
#define INCLUDED_BEMAN_EXECUTION26_DETAIL_AS_EXCEPT_PTR


// ----------------------------------------------------------------------------

namespace Beman::Execution26::Detail
{
    template <typename Error>
    decltype(auto) as_except_ptr(Error&& error)
    {
        if constexpr (::std::same_as<::std::exception_ptr, ::std::decay_t<Error>>)
        {
            assert(error != ::std::exception_ptr());
            return ::std::forward<Error>(error);
        }
        else if constexpr (::std::same_as<::std::error_code, ::std::decay_t<Error>>)
        {
            return ::std::make_exception_ptr(::std::system_error(error));
        }
        else
        {
            return ::std::make_exception_ptr(::std::forward<Error>(error));
        }
    }
}

// ----------------------------------------------------------------------------

#endif
// include/Beman/Execution26/detail/common.hpp                        -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef INCLUDED_BEMAN_EXECUTION26_DETAIL_COMMON
#define INCLUDED_BEMAN_EXECUTION26_DETAIL_COMMON

// ----------------------------------------------------------------------------

#if defined(__cpp_deleted_function)
#    define BEMAN_EXECUTION26_DELETE(msg) delete(msg)
#else
#    define BEMAN_EXECUTION26_DELETE(msg) delete
#endif

// ----------------------------------------------------------------------------

#endif
// include/Beman/Execution26/detail/simple_allocator.hpp              -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef INCLUDED_BEMAN_EXECUTION26_DETAIL_SIMPLE_ALLOCATOR
#define INCLUDED_BEMAN_EXECUTION26_DETAIL_SIMPLE_ALLOCATOR


// ----------------------------------------------------------------------------

namespace Beman::Execution26::Detail
{
    template <typename Alloc = bool>
    concept simple_allocator
        = requires(Alloc alloc, ::std::size_t n)
        {
            { *alloc.allocate(n) } -> ::std::same_as<typename Alloc::value_type&>;
            alloc.deallocate(alloc.allocate(n), n);
        }
        && ::std::copy_constructible<Alloc>
        && ::std::equality_comparable<Alloc>
        ;
}

// ----------------------------------------------------------------------------

#endif
// include/Beman/Execution26/detail/never_stop_token.hpp              -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef INCLUDED_BEMAN_EXECUTION26_DETAIL_NEVER_STOP_TOKEN
#define INCLUDED_BEMAN_EXECUTION26_DETAIL_NEVER_STOP_TOKEN

// ----------------------------------------------------------------------------

namespace Beman::Execution26
{
    class never_stop_token;
}

// ----------------------------------------------------------------------------

class Beman::Execution26::never_stop_token
{
    struct private_callback_type
    {
        explicit private_callback_type(never_stop_token, auto&&) noexcept {}
    };
public:
    template <typename>
    using callback_type = private_callback_type;

    static constexpr auto stop_requested() noexcept  -> bool { return {}; }
    static constexpr auto stop_possible() noexcept  -> bool { return {}; }
    auto operator== (never_stop_token const&) const -> bool = default;
};

// ----------------------------------------------------------------------------

#endif
// include/Beman/Execution26/detail/check_type_alias_exist.hpp        -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef INCLUDED_BEMAN_EXECUTION26_DETAIL_CHECK_TYPE_ALIAS_EXIST
#define INCLUDED_BEMAN_EXECUTION26_DETAIL_CHECK_TYPE_ALIAS_EXIST

// ----------------------------------------------------------------------------

namespace Beman::Execution26::Detail
{
    template <template<typename> class> struct check_type_alias_exist;
}

// ----------------------------------------------------------------------------

#endif
// include/Beman/Execution26/detail/callable.hpp                      -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef INCLUDED_BEMAN_EXECUTION26_DETAIL_CALLABLE
#define INCLUDED_BEMAN_EXECUTION26_DETAIL_CALLABLE


// ----------------------------------------------------------------------------

namespace Beman::Execution26::Detail
{
    template <typename Fun, typename... Args>
    concept callable
        = requires(Fun&& fun, Args&&... args)
        {
            ::std::forward<Fun>(fun)(::std::forward<Args>(args)...);
        }
        ;
}

// ----------------------------------------------------------------------------

#endif
// include/Beman/Execution26/detail/decayed_typeof.hpp                -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef INCLUDED_BEMAN_EXECUTION26_DETAIL_DECAYED_TYPEOF
#define INCLUDED_BEMAN_EXECUTION26_DETAIL_DECAYED_TYPEOF

// ----------------------------------------------------------------------------

namespace Beman::Execution26::Detail
{
    template <auto const& Tag>
    // using decayed_typeof = decltype(auto(Tag));
    using decayed_typeof = decltype([]{ return Tag; }());
}

// ----------------------------------------------------------------------------

#endif
// include/Beman/Execution26/detail/call_result_t.hpp                 -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef INCLUDED_BEMAN_EXECUTION26_DETAIL_CALL_RESULT
#define INCLUDED_BEMAN_EXECUTION26_DETAIL_CALL_RESULT


// ----------------------------------------------------------------------------

namespace Beman::Execution26::Detail
{
    template <typename Fun, typename... Args>
    using call_result_t = decltype(::std::declval<Fun>()(std::declval<Args>()...));
}

// ----------------------------------------------------------------------------

#endif
// include/Beman/Execution26/detail/get_allocator.hpp                 -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef INCLUDED_BEMAN_EXECUTION26_DETAIL_GET_ALLOCATOR
#define INCLUDED_BEMAN_EXECUTION26_DETAIL_GET_ALLOCATOR


// ----------------------------------------------------------------------------

namespace Beman::Execution26
{
    struct get_allocator_t
    {
        template <typename Object>
            requires (not requires(Object&& object, get_allocator_t const& tag)
            {
                ::std::as_const(object).query(tag);
            })
        auto operator()(Object&&) const
            = BEMAN_EXECUTION26_DELETE("the object requires a const query(get_allocator_t) overload");
        template <typename Object>
            requires (not requires(Object const& object, get_allocator_t const& tag)
            {
                object.query(tag);
            })
        auto operator()(Object&&) const
            = BEMAN_EXECUTION26_DELETE("the object requires a query(get_allocator_t) overload");
        template <typename Object>
            requires (not requires(Object const& object, get_allocator_t const& tag)
            {
                { object.query(tag) } noexcept;
            })
        auto operator()(Object&&) const
            = BEMAN_EXECUTION26_DELETE("the query(get_allocator_t) overload needs to be noexcept");
        template <typename Object>
            requires (not requires(Object const& object, get_allocator_t const& tag)
            {
                { object.query(tag) } noexcept -> ::Beman::Execution26::Detail::simple_allocator<>;
            })
        auto operator()(Object&&) const
            = BEMAN_EXECUTION26_DELETE("the query(get_allocator_t) overload needs to be noexcept");

        template <typename Object>
        auto operator()(Object&& object) const
        {
            return ::std::as_const(object).query(*this);
        }

        constexpr auto query(::Beman::Execution26::forwarding_query_t const&) const noexcept -> bool
        {
            return true;
        }
    };

    inline constexpr get_allocator_t get_allocator{};
}

// ----------------------------------------------------------------------------

#endif
// include/Beman/Execution26/detail/get_env.hpp                       -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef INCLUDED_BEMAN_EXECUTION26_DETAIL_GET_ENV
#define INCLUDED_BEMAN_EXECUTION26_DETAIL_GET_ENV


// ----------------------------------------------------------------------------

namespace Beman::Execution26
{
    struct empty_env {};

    struct get_env_t
    {
        template <typename Object>
            requires(
                not requires(Object&& object) { ::std::as_const(object).get_env(); }
                || ::Beman::Execution26::Detail::queryable<std::remove_cvref_t<decltype(::std::declval<::std::remove_cvref_t<Object> const&>().get_env())>>
            )
        auto operator()(Object&& object) const -> decltype(auto)
        {
            if constexpr (requires{ ::std::as_const(object).get_env(); })
            {
                static_assert(noexcept(::std::as_const(object).get_env()),
                              "get_env requires the xpression to be noexcept");
                static_assert(::Beman::Execution26::Detail::queryable<std::remove_cvref_t<decltype(::std::as_const(object).get_env())>>,
                              "get_env requires the result type to be destructible");
                return ::std::as_const(object).get_env();
            }
            else
            {
                return ::Beman::Execution26::empty_env{};
            }
        }
    };

    inline constexpr get_env_t get_env{};
}

// ----------------------------------------------------------------------------

#endif
// include/Beman/Execution26/detail/get_domain.hpp                    -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef INCLUDED_BEMAN_EXECUTION26_DETAIL_GET_DOMAIN
#define INCLUDED_BEMAN_EXECUTION26_DETAIL_GET_DOMAIN


// ----------------------------------------------------------------------------

namespace Beman::Execution26
{
    struct get_domain_t
    {
        template <typename Object>
            requires (not requires(Object&& object, get_domain_t const& tag)
            {
                ::std::forward<Object>(object).query(tag);
            })
            && (not requires(Object&& object, get_domain_t const& tag)
            {
                ::std::as_const(object).query(tag);
            })
        auto operator()(Object&&) const noexcept
            = BEMAN_EXECUTION26_DELETE("object neeeds a query(get_domain_t) overload");
        template <typename Object>
            requires (not requires(Object&& object, get_domain_t const& tag)
            {
                ::std::as_const(object).query(tag);
            })
        auto operator()(Object&&) const noexcept
            = BEMAN_EXECUTION26_DELETE("query(get_domain_t) overload needs to be const");
        template <typename Object>
            requires (not requires(Object&& object, get_domain_t const& tag)
            {
                { ::std::as_const(object).query(tag) } noexcept;
            })
        auto operator()(Object&&) const noexcept
            = BEMAN_EXECUTION26_DELETE("query(get_domain_t) overload needs to be noexcept");

        template <typename Object>
        constexpr auto operator()(Object&& object) const noexcept
        {
            return ::std::as_const(object).query(*this);
        }
        constexpr auto query(::Beman::Execution26::forwarding_query_t const&) const noexcept -> bool
        {
            return true;
        }
    };

    inline constexpr get_domain_t get_domain{};
}

// ----------------------------------------------------------------------------

#endif
// include/Beman/Execution26/detail/receiver.hpp                      -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef INCLUDED_BEMAN_EXECUTION26_DETAIL_RECEIVER
#define INCLUDED_BEMAN_EXECUTION26_DETAIL_RECEIVER


// ----------------------------------------------------------------------------

namespace Beman::Execution26
{
    struct receiver_t {};

    template <typename Rcvr>
    concept receiver
        =  ::std::derived_from<typename ::std::remove_cvref_t<Rcvr>::receiver_concept,
                             ::Beman::Execution26::receiver_t>
        && requires(::std::remove_cvref_t<Rcvr> const& rcvr)
            {
                { ::Beman::Execution26::get_env(rcvr) } -> ::Beman::Execution26::Detail::queryable;
            }
        && ::std::move_constructible<::std::remove_cvref_t<Rcvr>>
        && ::std::constructible_from<::std::remove_cvref_t<Rcvr>, Rcvr>
        && (not ::std::is_final_v<::std::remove_cvref_t<Rcvr>>)
        ;
}

// ----------------------------------------------------------------------------

#endif
// include/Beman/Execution26/detail/set_value.hpp -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef INCLUDED_BEMAN_EXECUTION26_DETAIL_SET_VALUE
#define INCLUDED_BEMAN_EXECUTION26_DETAIL_SET_VALUE


// ----------------------------------------------------------------------------

namespace Beman::Execution26
{
    struct set_value_t
    {
        template <typename Receiver, typename... Args>
        auto operator()(Receiver&, Args&&...) const -> void
            = BEMAN_EXECUTION26_DELETE("set_value requires the receiver to be passed as non-const rvalue");
        template <typename Receiver, typename... Args>
        auto operator()(Receiver const&&, Args&&...) const -> void
            = BEMAN_EXECUTION26_DELETE("set_value requires the receiver to be passed as non-const rvalue");
        template <typename Receiver, typename... Args>
        auto operator()(Receiver&&, Args&&...) const -> void
            requires (not requires(Receiver&& receiver, Args&&... args)
            {
                ::std::forward<Receiver>(receiver).set_value(::std::forward<Args>(args)...);
            })
            = BEMAN_EXECUTION26_DELETE("set_value requires a suitable member overload on the receiver");
        template <typename Receiver, typename... Args>
            requires (not noexcept(::std::declval<Receiver>().set_value(::std::declval<Args>()...)))
        auto operator()(Receiver&&, Args&&...) const -> void
            = BEMAN_EXECUTION26_DELETE("the call to receiver.set_value(args...) has to be noexcept");

        template <typename Receiver, typename... Args>
        auto operator()(Receiver&& receiver, Args&&... args) const noexcept -> void
        {
            ::std::forward<Receiver>(receiver).set_value(::std::forward<Args>(args)...);
        }
    };
    inline constexpr set_value_t set_value{};
}

// ----------------------------------------------------------------------------

#endif
// inclue/Beman/Execution26/detail/set_error.hpp -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef INCLUDED_BEMAN_EXECUTION26_DETAIL_SET_ERROR
#define INCLUDED_BEMAN_EXECUTION26_DETAIL_SET_ERROR


// ----------------------------------------------------------------------------

namespace Beman::Execution26
{
    struct set_error_t
    {
        template <typename Receiver, typename Error>
        auto operator()(Receiver&, Error&&) const -> void
            = BEMAN_EXECUTION26_DELETE("set_error requires the receiver to be passed as non-const rvalue");
        template <typename Receiver, typename Error>
        auto operator()(Receiver const&&, Error&&) const -> void
            = BEMAN_EXECUTION26_DELETE("set_error requires the receiver to be passed as non-const rvalue");
        template <typename Receiver, typename Error>
        auto operator()(Receiver&&, Error&&) const -> void
            requires (not requires(Receiver&& receiver, Error&& error)
            {
                ::std::forward<Receiver>(receiver).set_error(::std::forward<Error>(error));
            })
            = BEMAN_EXECUTION26_DELETE("set_error requires a suitable member overload on the receiver");
        template <typename Receiver, typename Error>
            requires (not noexcept(::std::declval<Receiver>().set_error(::std::declval<Error>())))
        auto operator()(Receiver&&, Error&&) const -> void
            = BEMAN_EXECUTION26_DELETE("the call to receiver.set_error(error) has to be noexcept");


        template <typename Receiver, typename Error>
        auto operator()(Receiver&& receiver, Error&& error) const noexcept -> void
        {
            ::std::forward<Receiver>(receiver).set_error(::std::forward<Error>(error));
        }
    };
    
    inline constexpr set_error_t set_error{};
}

// ----------------------------------------------------------------------------

#endif
// include/Beman/Execution26/detail/set_stopped.hpp                   -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef INCLUDED_BEMAN_EXECUTION26_DETAIL_SET_STOPPED
#define INCLUDED_BEMAN_EXECUTION26_DETAIL_SET_STOPPED


// ----------------------------------------------------------------------------

namespace Beman::Execution26
{
    struct set_stopped_t
    {
        template <typename Receiver>
        auto operator()(Receiver&) const -> void
            = BEMAN_EXECUTION26_DELETE("set_stopped requires the receiver to be passed as non-const rvalue");
        template <typename Receiver>
        auto operator()(Receiver const&&) const -> void
            = BEMAN_EXECUTION26_DELETE("set_stopped requires the receiver to be passed as non-const rvalue");
        template <typename Receiver>
        auto operator()(Receiver&&) const -> void
            requires (not requires(Receiver&& receiver)
            {
                ::std::forward<Receiver>(receiver).set_stopped();
            })
            = BEMAN_EXECUTION26_DELETE("set_stopped requires a suitable member overload on the receiver");
        template <typename Receiver>
            requires (not noexcept(::std::declval<Receiver>().set_stopped()))
        auto operator()(Receiver&&) const -> void
            = BEMAN_EXECUTION26_DELETE("the call to receiver.set_stopped() has to be noexcept");

        template <typename Receiver>
        auto operator()(Receiver&& receiver) const noexcept -> void
        {
            ::std::forward<Receiver>(receiver).set_stopped();
        }
    };

    inline constexpr set_stopped_t set_stopped{};
}

// ----------------------------------------------------------------------------

#endif
// include/Beman/Execution26/detail/start.hpp                         -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef INCLUDED_BEMAN_EXECUTION26_DETAIL_START
#define INCLUDED_BEMAN_EXECUTION26_DETAIL_START


// ----------------------------------------------------------------------------

namespace Beman::Execution26
{
    struct start_t
    {
        template <typename State>
        auto operator()(State&&) const -> void
            = BEMAN_EXECUTION26_DELETE("start(obj) requires an lvalue argument");
        template <typename State>
            requires (not requires(State& state){ state.start(); })
        auto operator()(State&) const -> void
            = BEMAN_EXECUTION26_DELETE("state needs to have a start() member");
        template <typename State>
            requires (not requires(State const& state){ state.start(); })
        auto operator()(State const&) const -> void
            = BEMAN_EXECUTION26_DELETE("state needs to have a start() member");

        template <typename State>
            requires (not requires(State& state){ { state.start() } noexcept; })
        auto operator()(State&) const -> void
            = BEMAN_EXECUTION26_DELETE("state start() member has to be noexcept");
        template <typename State>
            requires (not requires(State const& state){ { state.start()} noexcept; })
        auto operator()(State const&) const -> void
            = BEMAN_EXECUTION26_DELETE("state start() member has to be noexcept");

        template <typename State>
        auto operator()(State const& state) const noexcept -> void
        {
            state.start();
        }
        template <typename State>
        auto operator()(State& state) const noexcept -> void
        {
            state.start();
        }
    };

    inline constexpr start_t start{};
}

// ----------------------------------------------------------------------------

#endif
// include/Beman/Execution26/detail/stoppable_token.hpp               -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef INCLUDED_BEMAN_EXECUTION26_DETAIL_STOPPABLE_TOKEN
#define INCLUDED_BEMAN_EXECUTION26_DETAIL_STOPPABLE_TOKEN


// ----------------------------------------------------------------------------

namespace Beman::Execution26
{
    template<typename Token>
    concept stoppable_token
        =   requires(Token const& token)
            {
                typename ::Beman::Execution26::Detail::check_type_alias_exist<Token::template callback_type>;
                { token.stop_requested() } noexcept -> ::std::same_as<bool>;
                { token.stop_possible() } noexcept -> ::std::same_as<bool>;
                { Token(token) } noexcept;
            }
        &&  ::std::copyable<Token>
        &&  ::std::equality_comparable<Token>
        &&  ::std::swappable<Token>
        ;
}

// ----------------------------------------------------------------------------

#endif
// include/Beman/Execution26/detail/nothrow_callable.hpp              -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef INCLUDED_BEMAN_EXECUTION26_DETAIL_NOTHROW_CALLABLE
#define INCLUDED_BEMAN_EXECUTION26_DETAIL_NOTHROW_CALLABLE


// ----------------------------------------------------------------------------

namespace Beman::Execution26::Detail
{
    template <typename Fun, typename... Args>
    concept nothrow_callable
        =  ::Beman::Execution26::Detail::callable<Fun, Args...>
        && requires(Fun&& fun, Args&&... args)
        {
            { ::std::forward<Fun>(fun)(::std::forward<Args>(args)...) } noexcept;
        }
        ;
}

// ----------------------------------------------------------------------------

#endif
// include/Beman/Execution26/detail/get_stop_token.hpp                -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef INCLUDED_BEMAN_EXECUTION26_DETAIL_GET_STOP_TOKEN
#define INCLUDED_BEMAN_EXECUTION26_DETAIL_GET_STOP_TOKEN


// ----------------------------------------------------------------------------

namespace Beman::Execution26
{
    struct get_stop_token_t
    {
        template <typename Object>
            requires requires(Object&& object, get_stop_token_t const& tag)
            {
                { ::std::as_const(object).query(tag) } noexcept -> ::Beman::Execution26::stoppable_token;
            }
        auto operator()(Object&& object) const noexcept
        {
            return ::std::as_const(object).query(*this);
        }

        template <typename Object>
        auto operator()(Object&&) const noexcept -> ::Beman::Execution26::never_stop_token
        {
            return {};
        }

        constexpr auto query(::Beman::Execution26::forwarding_query_t const&) const noexcept -> bool
        {
            return true;
        }
    };

    inline constexpr get_stop_token_t get_stop_token{};
}

// ----------------------------------------------------------------------------

#endif
// include/Beman/Execution26/detail/completion_signature.hpp          -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef INCLUDED_BEMAN_EXECUTION26_DETAIL_COMPLETION_SIGNATURE
#define INCLUDED_BEMAN_EXECUTION26_DETAIL_COMPLETION_SIGNATURE


// ----------------------------------------------------------------------------

namespace Beman::Execution26::Detail
{
    template <typename> struct is_set_error : ::std::false_type {};
    template <typename Error>
    struct is_set_error<::Beman::Execution26::set_error_t(Error)>
        : ::std::true_type
    {
    };

    template <typename> struct is_set_stopped: ::std::false_type {};
    template <>
    struct is_set_error<::Beman::Execution26::set_stopped_t()>
        : ::std::true_type
    {
    };

    template <typename> struct is_set_value: ::std::false_type {};
    template <typename... Args>
    struct is_set_value<::Beman::Execution26::set_value_t(Args...)>
        : ::std::true_type
    {
    };

    template <typename Fun>
    concept completion_signature
        =  ::Beman::Execution26::Detail::is_set_error<Fun>::value
        || ::Beman::Execution26::Detail::is_set_stopped<Fun>::value
        || ::Beman::Execution26::Detail::is_set_value<Fun>::value
        ;
}


// ----------------------------------------------------------------------------

#endif
// include/Beman/Execution26/functional.hpp                           -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef INCLUDED_BEMAN_EXECUTION26_FUNCTIONAL
#define INCLUDED_BEMAN_EXECUTION26_FUNCTIONAL

// ----------------------------------------------------------------------------


// ----------------------------------------------------------------------------

#endif
// include/Beman/Execution26/detail/completion_signatures.hpp         -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef INCLUDED_BEMAN_EXECUTION26_DETAIL_COMPLETION_SIGNATURES
#define INCLUDED_BEMAN_EXECUTION26_DETAIL_COMPLETION_SIGNATURES


// ----------------------------------------------------------------------------

namespace Beman::Execution26
{
    template <::Beman::Execution26::Detail::completion_signature...>
    struct completion_signatures
    {
    };
}

// ----------------------------------------------------------------------------

#endif
// include/Beman/Execution26/detail/valid_completion_for.hpp          -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef INCLUDED_BEMAN_EXECUTION26_DETAIL_VALID_COMPLETION_FOR
#define INCLUDED_BEMAN_EXECUTION26_DETAIL_VALID_COMPLETION_FOR


// ----------------------------------------------------------------------------

namespace Beman::Execution26::Detail
{
    template <typename, typename> struct valid_completion_for_aux;

    template <typename Rcvr, typename Tag, typename... Args>
    struct valid_completion_for_aux<Rcvr, Tag(*)(Args...)>
    {
        static auto test(Tag(*)(Args...)) -> void
            requires ::Beman::Execution26::Detail::callable<Tag, ::std::remove_cvref_t<Rcvr>, Args...>
        {
        }
    };

    template <typename Signature, typename Rcvr>
    concept valid_completion_for
        = requires(Signature* signature)
        {
            #if 1
            valid_completion_for_aux<Rcvr, Signature*>::test(signature);
            #else
            // This definition crashes some versions of clang.
            []<typename Tag, typename... Args>(Tag(*)(Args...))
                requires ::Beman::Execution26::Detail::callable<Tag, ::std::remove_cvref_t<Rcvr>, Args...>
            {
            }(signature);
            #endif
        }
        ;
}

// ----------------------------------------------------------------------------

#endif
// include/Beman/Execution26/detail/valid_completion_signatures.hpp   -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef INCLUDED_BEMAN_EXECUTION26_DETAIL_VALID_COMPLETION_SIGNATURES
#define INCLUDED_BEMAN_EXECUTION26_DETAIL_VALID_COMPLETION_SIGNATURES


// ----------------------------------------------------------------------------

namespace Beman::Execution26::Detail
{
    template <typename>
    struct valid_completion_signatures_helper
        : ::std::false_type
    {
    };
    template <typename... Sigs>
    struct valid_completion_signatures_helper<::Beman::Execution26::completion_signatures<Sigs...>>
        : ::std::true_type
    {
    };

    template <typename Signatures>
    concept valid_completion_signatures
        = valid_completion_signatures_helper<Signatures>::value
        ;
}

// ----------------------------------------------------------------------------

#endif


// ----------------------------------------------------------------------------




// ----------------------------------------------------------------------------

#endif
