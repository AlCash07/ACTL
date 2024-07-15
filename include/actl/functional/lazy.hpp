// Copyright 2024 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/functional/noexcept/AC_DEDUCE_NOEXCEPT_AND_RETURN.hpp>
#include <actl/memory/AC_NO_UNIQUE_ADDRESS.hpp>
#include <concepts>

namespace ac {

/// Wrapper to lazily evaluate a function (which has no parameters)
/// only when the result is needed,
/// specifically when conversion to the result is requested.
///
/// This simple wrapper has surprisingly remarkable benefits:
///
/// 1. It simplifies the code that uses the function result only conditionally,
/// for example, when inserting an element into a map
/// only if it doesn't already exist: @code
/// map.try_emplace(key, ac::lazy{computeValue});
/// @endcode
/// If we want the function to be called only when necessary,
/// the shortest alternative looks like this: @code
/// auto iter = map.find(key);
/// if (iter != map.end())
///     map.emplace_hint(iter, key, computeValue());
/// @endcode
///
/// 2. Even when the function is always called unconditionally,
/// approach similar to ac::lazy is the only way to avoid an
/// extra move constructor when emplacing a function result into a container.
/// If a move constructor is not defined, copy constructor is called instead.
///
/// For example, the first line here results
/// in the extra move constructor of the result,
/// while the second doesn't: @code
/// std::optional optional1{computeValue()};
/// std::optional optional2{ac::lazy{computeValue}};
/// @endcode
///
/// For the in-depth explanation, please refer to this article,
/// where the analog is called `with_result_of_t`
/// (I hope you'll find our naming better):
/// https://quuxplusone.github.io/blog/2018/05/17/super-elider-round-2/
///
/// Or to the original source of the idea:
/// https://akrzemi1.wordpress.com/2018/05/16/rvalues-redefined/
///
/// @note This is the simplest possible implementation,
/// so it doesn't perform any result caching
/// for the case where it can be used multiple times.
template<std::invocable Function>
struct lazy {
    /// Function result can be constructed from ac::lazy
    /// thanks to this conversion operator.
    operator std::invoke_result_t<Function>()
        AC_DEDUCE_NOEXCEPT_AND_RETURN(function())

    AC_NO_UNIQUE_ADDRESS Function function;
};

template<std::invocable Function>
lazy(Function&&) -> lazy<Function>;

} // namespace ac
