// Copyright 2026 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt)

#pragma once

namespace ac {

/// Macro to be used inside a class to disable its
/// copy constructor and assignment operator
/// while allowing to define custom
/// move constructor and assignment operator.
/// For example,
/// @code
/// class unique_ptr {
///     void* pointer = nullptr;
/// public:
///     unique_ptr() = default;
///     unique_ptr(unique_ptr&& that) {
///         this = std::move(*that);
///     }
///     unique_ptr& operator=(unique_ptr&& that) {
///         this->poiner = that->pointer;
///         that->poiner = nullptr;
///         return *this;
///     }
///     AC_DISABLE_COPY(unique_ptr)
/// };
///
/// unique_ptr a;
/// unique_ptr b = a;            // compilation error
/// unique_ptr c = std::move(a); // ok
/// @endcode
///
/// Default constructor is also disabled unless defined explicitly.
///
/// @note Move constructor and assignment operator are disabled if not defined.
/// If that's the intention then it's more descriptive to use AC_DISABLE_MOVE.
#define AC_DISABLE_COPY(type)   \
    type(const type&) = delete; \
    type& operator=(const type&) = delete;

} // namespace ac
