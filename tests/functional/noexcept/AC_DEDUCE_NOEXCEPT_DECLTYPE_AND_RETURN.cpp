// Copyright 2022 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt)

#include <actl/functional/noexcept/AC_DEDUCE_NOEXCEPT_DECLTYPE_AND_RETURN.hpp>
#include "functional/noexcept/is_int.hpp"

template<class T>
constexpr auto is_int_wrapped(T x)
    AC_DEDUCE_NOEXCEPT_DECLTYPE_AND_RETURN(ac::is_int(x))

static_assert(noexcept(is_int_wrapped(0)));
static_assert(is_int_wrapped(0));
static_assert(!noexcept(is_int_wrapped(-1.0)));
static_assert(!is_int_wrapped(0.0));

template<class T>
concept has_is_int_wrapped = requires(T x) { is_int_wrapped(x); };

static_assert(has_is_int_wrapped<int>);
// This would be a compilation error with AC_DEDUCE_NOEXCEPT_AND_RETURN.
static_assert(!has_is_int_wrapped<int[2]>);
