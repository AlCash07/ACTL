// Copyright 2024 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/functional/noexcept/AC_DEDUCE_NOEXCEPT_AND_RETURN.hpp>
#include <actl/functional/traits/Callable.hpp>
#include <actl/memory/AC_NO_UNIQUE_ADDRESS.hpp>

namespace ac {

/// Wrapper to lazily evaluate a function (with no parameters)
/// only when the result is needed.
///
/// This simple wrapper has surprisingly remarkable benefits:
///
/// 1. Simpler and more efficient code when the function result is used
/// conditionally, for example, when inserting an element into a map
/// only if it doesn't already exist:
/// @code
/// auto iter = map.try_emplace(key, ac::lazy{computeValue}).first;
/// @endcode
/// The shortest alternative looks like this,
/// and does an extra pass over the map:
/// @code
/// auto iter = map.find(key);
/// if (iter == map.end())
///     iter = map.emplace(key, computeValue()).first;
/// @endcode
///
/// 2. Even when the function is called unconditionally,
/// approach similar to ac::lazy is the only way to avoid an
/// extra move constructor when emplacing a function result into a container.
/// If a move constructor is not defined, copy constructor is called instead.
///
/// For example, the first line here results
/// in the extra move constructor of the result,
/// while the second doesn't:
/// @code
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
template<Callable Function>
struct lazy {
    AC_NO_UNIQUE_ADDRESS Function function;

    /// Function result can be constructed from ac::lazy
    /// thanks to this conversion operator.
    operator decltype(function())() AC_DEDUCE_NOEXCEPT_AND_RETURN( //
        function()
    )
};

template<Callable Function>
lazy(Function&&) -> lazy<Function>;

} // namespace ac
