// include/Beman/Execution26/functional.hpp                           -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef INCLUDED_BEMAN_EXECUTION26_FUNCTIONAL
#define INCLUDED_BEMAN_EXECUTION26_FUNCTIONAL

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


// ----------------------------------------------------------------------------


// ----------------------------------------------------------------------------

#endif
