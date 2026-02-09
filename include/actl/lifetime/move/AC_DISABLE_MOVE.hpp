// Copyright 2026 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt)

#pragma once

/// Macro to be used inside a class to disable its
/// move and copy constructor and assignment operator.
/// For example,
/// @code
/// class mutex {
/// public:
///     mutex() = default;
///     AC_DISABLE_MOVE(mutex)
/// };
///
/// mutex a;
/// mutex b = a;            // compilation error
/// mutex c = std::move(a); // compilation error
/// @endcode
///
/// Default constructor is also disabled unless defined explicitly.
///
/// To disable only copy and not move, use AC_DISABLE_COPY.
#define AC_DISABLE_MOVE(type)         \
    type(type&&) = delete;            \
    type& operator=(type&&) = delete; \
                                      \
    type(const type&) = delete;       \
    type& operator=(const type&) = delete;
