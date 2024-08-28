// include/beman/execution26/detail/get_completion_signatures.hpp     -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef INCLUDED_BEMAN_EXECUTION26_DETAIL_GET_COMPLETION_SIGNATURES
#define INCLUDED_BEMAN_EXECUTION26_DETAIL_GET_COMPLETION_SIGNATURES

#include <beman/execution26/detail/transform_sender.hpp> //-dk:TODO remove
#include <beman/execution26/detail/get_domain_late.hpp> //-dk:TODO remove
#include <concepts>
#include <type_traits>
#include <utility>

// ----------------------------------------------------------------------------

namespace beman::execution26
{
    struct get_completion_signatures_t
    {
    private:
        template <typename Sender, typename Env>
        static auto get(Sender&& sender, Env&& env) noexcept
        {
            auto new_sender{[](auto&& sender, auto&& env) -> decltype(auto) {
                return ::beman::execution26::transform_sender(
                    ::beman::execution26::detail::get_domain_late(sender, env),
                    ::std::forward<Sender>(sender),
                    ::std::forward<Env>(env)
                );
            }};

            using sender_type = ::std::remove_cvref_t<decltype(new_sender(sender, env))>;
            if constexpr (requires{ new_sender(sender, env).get_completion_signatures(env); })
                return decltype(new_sender(sender, env).get_completion_signatures(env)){};
            else if constexpr (requires{ typename sender_type::completion_signatures; })
                return typename sender_type::completion_signatures{};
            //-dk:TODO deal with awaitable
        }
    public:
        template <typename Sender, typename Env>
            requires (not ::std::same_as<void,
                decltype(get_completion_signatures_t::get(::std::declval<Sender>(), ::std::declval<Env>()))>
            )
        auto operator()(Sender&& sender, Env&& env) const noexcept
        {
            return this->get(::std::forward<Sender>(sender), ::std::forward<Env>(env));
        }
    };
    inline constexpr get_completion_signatures_t get_completion_signatures{};
}

// ----------------------------------------------------------------------------

#endif