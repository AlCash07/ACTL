// Copyright 2026 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/sequence/tuple/detail/tuple_element.hpp>

namespace ac::detail {

#define AC_TUPLE_AT(CV_REF)                                                \
    template<size_t Index, typename T>                                     \
        requires(not EmptyTrivial<T>)                                      \
    constexpr T CV_REF at(tuple_element<Index, T> CV_REF tuple) noexcept { \
        return static_cast<T CV_REF>(tuple.m_element);                     \
    }

AC_TUPLE_AT(&)
AC_TUPLE_AT(&&)
AC_TUPLE_AT(const&)
AC_TUPLE_AT(const&&)
#undef AC_TUPLE_AT

template<size_t Index, EmptyTrivial Empty>
constexpr Empty at(tuple_element<Index, Empty>) noexcept {
    // EmptyTrivial objects can be efficiently created on the fly.
    return Empty{};
}

} // namespace ac::detail
