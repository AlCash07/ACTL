// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/functional/deduce_noexcept.hpp>
#include <actl/meta/dependent.hpp>

namespace ac {

struct iterator_core_access
{
    template <class Iter>
    static constexpr reference_t<Iter> dereference(const Iter& iter)
        AC_DEDUCE_NOEXCEPT_AND_RETURN(iter.dereference())

    template <class Iter>
    static constexpr void increment(Iter& iter)
        AC_DEDUCE_NOEXCEPT_AND_RETURN(iter.increment())

    template <class Iter>
    static constexpr void decrement(Iter& iter)
        AC_DEDUCE_NOEXCEPT_AND_RETURN(iter.decrement())

    template <class Iter1, class Iter2>
    static constexpr bool equal(const Iter1& lhs, const Iter2& rhs)
        AC_DEDUCE_NOEXCEPT_AND_RETURN(lhs.equals(rhs))

    template <class Iter>
    static constexpr void advance(Iter& iter, difference_type_t<Iter> n)
        AC_DEDUCE_NOEXCEPT_AND_RETURN(iter.advance(n))

    template <class Iter1, class Iter2>
    static constexpr difference_type_t<Iter1> distance_to(
        const Iter1& from, const Iter2& to)
        AC_DEDUCE_NOEXCEPT_AND_RETURN(from.distance_to(to))
};

} // namespace ac
