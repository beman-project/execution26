// include/beman/execution26/detail/apply_sender.hpp                  -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef INCLUDED_BEMAN_EXECUTION26_DETAIL_APPLY_SENDER
#define INCLUDED_BEMAN_EXECUTION26_DETAIL_APPLY_SENDER

#include <beman/execution26/detail/default_domain.hpp>
#include <beman/execution26/detail/sender.hpp>
#include <utility>

// ----------------------------------------------------------------------------

namespace beman::execution26 {
/*!
 * \brief Function used to transform a sender and its arguments for a domain.
 * \headerfile beman/execution26/execution.hpp <beman/execution26/execution.hpp>
 */
template <typename Domain, typename Tag, ::beman::execution26::sender Sender, typename... Args>
    requires requires(Domain domain, Tag tag, Sender&& sender, Args&&... args) {
        domain.apply_sender(Tag(), ::std::forward<Sender>(sender), ::std::forward<Args>(args)...);
    }
constexpr auto apply_sender(Domain domain, Tag, Sender&& sender, Args&&... args) noexcept(noexcept(
    domain.apply_sender(Tag(), ::std::forward<Sender>(sender), ::std::forward<Args>(args)...))) -> decltype(auto) {
    return domain.apply_sender(Tag(), ::std::forward<Sender>(sender), ::std::forward<Args>(args)...);
}

/*!
 * \brief Default function used to transform a second and its arguments.
 * \headerfile beman/execution26/execution.hpp <beman/execution26/execution.hpp>
 */
template <typename Domain, typename Tag, ::beman::execution26::sender Sender, typename... Args>
    requires(not requires(Domain domain, Tag tag, Sender&& sender, Args&&... args) {
                domain.apply_sender(Tag(), ::std::forward<Sender>(sender), ::std::forward<Args>(args)...);
            }) && requires(Tag tag, Sender&& sender, Args&&... args) {
        beman::execution26::default_domain().apply_sender(
            Tag(), ::std::forward<Sender>(sender), ::std::forward<Args>(args)...);
    }
constexpr auto apply_sender(Domain, Tag, Sender&& sender, Args&&... args) noexcept(
    noexcept(beman::execution26::default_domain().apply_sender(Tag(),
                                                               ::std::forward<Sender>(sender),
                                                               ::std::forward<Args>(args)...))) -> decltype(auto) {
    return beman::execution26::default_domain().apply_sender(
        Tag(), ::std::forward<Sender>(sender), ::std::forward<Args>(args)...);
}

} // namespace beman::execution26

// ----------------------------------------------------------------------------

#endif
