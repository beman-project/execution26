// src/Beman/Execution26/tests/exec-snd-transform.pass.cpp            -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include <Beman/Execution26/detail/transform_sender.hpp>
#include <Beman/Execution26/detail/sender.hpp>
#include <Beman/Execution26/execution.hpp>
#include <test/execution.hpp>
#include <concepts>
#include <type_traits>

// ----------------------------------------------------------------------------

namespace
{
    struct env {};
    struct empty_domain {};

    struct final_sender
    {
        using sender_concept = test_std::sender_t;
        int value{};
        auto operator== (final_sender const&) const -> bool = default;
    };

    template <int> struct sender;

    template <int I>
    struct tag
    {
        template <typename Sender, typename... Env>
        auto transform_sender(Sender s, Env&&...) const noexcept
        {
            if constexpr (1 < I)
                return sender<I - 1>{{}, s.value};
            else
                return final_sender{s.value};
        }
    };

    template <int I>
    struct sender
    {
        using sender_concept = test_std::sender_t;
        tag<I> t;
        int    value{};
    };

    template <bool Noexcept, typename Expect, typename Dom, typename Sender>
    auto test_transform(Dom&& dom, Sender&& sndr)
    {
        static_assert(test_std::sender<std::remove_cvref_t<Sender>>);
        static_assert(requires{ test_std::transform_sender(dom, std::forward<Sender>(sndr)); });
        if constexpr (requires{ test_std::transform_sender(dom, std::forward<Sender>(sndr)); })
        {
            static_assert(std::same_as<Expect,
                decltype(test_std::transform_sender(dom, std::forward<Sender>(sndr)))>);
            static_assert(Noexcept == noexcept(test_std::transform_sender(dom, std::forward<Sender>(sndr))));
            assert(sndr.value == test_std::transform_sender(dom, std::forward<Sender>(sndr)).value);
        }

        static_assert(requires{ test_std::transform_sender(dom, std::forward<Sender>(sndr), env{}); });
        if constexpr (requires{ test_std::transform_sender(dom, std::forward<Sender>(sndr), env{}); })
        {
            static_assert(std::same_as<Expect,
                decltype(test_std::transform_sender(dom, std::forward<Sender>(sndr), env{}))>);
            static_assert(Noexcept == noexcept(test_std::transform_sender(dom, std::forward<Sender>(sndr), env{})));
            assert(sndr.value == test_std::transform_sender(dom, std::forward<Sender>(sndr), env{}).value);
        }

        static_assert(not requires{ test_std::transform_sender(dom, std::forward<Sender>(sndr), env{}, env{}); });
    }
}

auto main() -> int
{
    test_transform<true, final_sender&&>(empty_domain{}, final_sender{42});
    final_sender fs{42};
    test_transform<true, final_sender&>(empty_domain{}, fs);
    final_sender const cfs{42};
    test_transform<true, final_sender const&>(empty_domain{}, cfs);

    static_assert(std::same_as<tag<1>, test_std::tag_of_t<sender<1>>>);
    static_assert(std::same_as<tag<2>, test_std::tag_of_t<sender<2>>>);

    static_assert(std::same_as<final_sender,
        decltype(tag<1>{}.transform_sender(sender<1>{{}, 0}))>);
    static_assert(std::same_as<final_sender,
        decltype(tag<1>{}.transform_sender(sender<1>{{}, 0}, env{}))>);
    static_assert(std::same_as<final_sender,
        decltype(test_std::default_domain{}.transform_sender(sender<1>{{}, 0}))>);
    static_assert(std::same_as<final_sender,
        decltype(test_std::default_domain{}.transform_sender(sender<1>{{}, 0}, env{}))>);

    static_assert(std::same_as<sender<1>,
        decltype(tag<2>{}.transform_sender(sender<2>{{}, 0}))>);
    static_assert(std::same_as<sender<1>,
        decltype(tag<2>{}.transform_sender(sender<2>{{}, 0}, env{}))>);

    auto s = test_std::transform_sender(empty_domain{}, sender<1>{{}, 42});
    static_assert(std::same_as<final_sender, decltype(s)>);
    test_transform<true, final_sender>(empty_domain{}, sender<1>{{}, 42});
}