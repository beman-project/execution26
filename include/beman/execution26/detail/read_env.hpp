// include/beman/execution26/detail/read_env.hpp                      -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef INCLUDED_BEMAN_EXECUTION26_DETAIL_READ_ENV
#define INCLUDED_BEMAN_EXECUTION26_DETAIL_READ_ENV

#include <beman/execution26/detail/completion_signatures.hpp>
#include <beman/execution26/detail/completion_signatures_for.hpp>
#include <beman/execution26/detail/decayed_typeof.hpp>
#include <beman/execution26/detail/default_impls.hpp>
#include <beman/execution26/detail/get_env.hpp>
#include <beman/execution26/detail/impls_for.hpp>
#include <beman/execution26/detail/make_sender.hpp>
#include <beman/execution26/detail/set_error.hpp>
#include <beman/execution26/detail/set_value.hpp>
#include <exception>
#include <type_traits>
#include <utility>

// ----------------------------------------------------------------------------

namespace beman::execution26::detail
{
    struct read_env_t
    {
        auto operator()(auto&& query) const
        {
            return ::beman::execution26::detail::make_sender(*this, query);
        }
    };

    template <>
    struct impls_for<::beman::execution26::detail::read_env_t>
        : ::beman::execution26::detail::default_impls
    {
        static constexpr auto start
            = [](auto query, auto& receiver) noexcept -> void
        {
            try
            {
                auto env{::beman::execution26::get_env(receiver)};
                ::beman::execution26::set_value(
                    ::std::move(receiver),
                    query(env)
                );
            }
            catch(...)
            {
                ::beman::execution26::set_error(
                    ::std::move(receiver),
                    ::std::current_exception()
                );
            }
            
        };
    };

    template <typename Query, typename Env>
    struct completion_signatures_for_impl<
        ::beman::execution26::detail::basic_sender<
            ::beman::execution26::detail::read_env_t,
            Query
            >,
        Env
        >
    {
        using type = ::beman::execution26::completion_signatures<
            ::beman::execution26::set_value_t(
                decltype(::std::declval<Query>()(::std::as_const(::std::declval<Env>())))
            ),
            ::beman::execution26::set_error_t(::std::exception_ptr)
            >;
    };
}

namespace beman::execution26
{
    using read_env_t = beman::execution26::detail::read_env_t;
    inline constexpr read_env_t read_env{};
}

// ----------------------------------------------------------------------------

#endif
