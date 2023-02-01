// Copyright 2022 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt)

#pragma once

/// Macro that avoids code duplication by using the given expression as a member
/// initializer list and deducing the `noexcept` specification from it.
/// For example, the following constructor @code
/// Derived(int x) noexcept(noexcept(Base{x})) : Base{x} {}
/// @endcode
/// can be rewritten as @code
/// Derived(int x) AC_DEDUCE_NOEXCEPT_AND_INITIALIZE(Base{x}) {}
/// @endcode
/// @note Data members aren't supported, only base classes and delegated
/// constructors.
#define AC_DEDUCE_NOEXCEPT_AND_INITIALIZE(...) \
    noexcept(noexcept(__VA_ARGS__)) : __VA_ARGS__
