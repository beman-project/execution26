// Copyright © 2024 Beman Project
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef INCLUDED_STOP_TOKEN
#define INCLUDED_STOP_TOKEN

#include <concepts>

namespace beman::inline cpp26
{
    template<class Token, class CallbackFun>
    using stop_callback_for_t = Token::template callback_type<CallbackFun>;

    namespace detail
    {
        template<typename CallbackFun, typename Token, typename Initializer = CallbackFun>
        concept stoppable_callback_for
            =  ::std::invocable<CallbackFun>
            && ::std::constructible_from<CallbackFun, Initializer>
            && requires { typename ::beman::cpp26::stop_callback_for_t<Token, CallbackFun>; }
            && ::std::constructible_from<::beman::cpp26::stop_callback_for_t<Token, CallbackFun>,
                                         Token,
                                         Initializer>
            && ::std::constructible_from<::beman::cpp26::stop_callback_for_t<Token, CallbackFun>,
                                         Token&,
                                         Initializer>
            && ::std::constructible_from<::beman::cpp26::stop_callback_for_t<Token, CallbackFun>,
                                         Token const&,
                                         Initializer>
            ;

        template<typename Source>
        concept stoppable_source = true;

        template <template<typename> class> struct check_type_alias_exist;
    }

    template<typename Token>
    concept stoppable_token
        =   requires(Token const& token)
            {
                typename ::beman::cpp26::detail::check_type_alias_exist<Token::template callback_type>;
                { token.stop_requested() } noexcept -> ::std::same_as<bool>;
                { token.stop_possible() } noexcept -> ::std::same_as<bool>;
                { Token(token) } noexcept;
            }
        &&  ::std::copyable<Token>
        &&  ::std::equality_comparable<Token>
        &&  ::std::swappable<Token>
        ;

    template<typename Token>
    concept unstoppable_token = false;

    class never_stop_token;

    class stop_token;
    class stop_source;
    template<typename CallbackFun> class stop_callback;

    struct nostopstate_t {
        explicit nostopstate_t() = default;
    };
    inline constexpr nostopstate_t nostopstate{};

    class inplace_stop_token;
    class inplace_stop_source;
    template<typename CallbackFun> class inplace_stop_callback;
}

#endif // INCLUDED_STOP_TOKEN
