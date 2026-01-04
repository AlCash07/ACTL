// Copyright 2026 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt)

#pragma once

namespace ac {

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
/// Effect is similar to boost::noncopyable
/// https://www.boost.org/doc/libs/latest/libs/core/doc/html/core/noncopyable.html
/// However, the base class approach of boost::noncopyable has these problems:
/// - more complex class hierarchy is introduced;
/// - namespace of the base class is added to ADL,
///   which adds extra work for the compiler
///   and can have surprising effects;
/// - if multiple empty classes inherit the same boost::noncopyable
///   then their storage cannot be optimized together using EBO.
#define AC_DISABLE_MOVE(type)         \
    type(type&&) = delete;            \
    type& operator=(type&&) = delete; \
                                      \
    type(const type&) = delete;       \
    type& operator=(const type&) = delete;

} // namespace ac
