// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/functional/deduce_noexcept.hpp>
#include <actl/range/traits/dependent.hpp>

namespace ac {

struct iterator_core_access
{
    template <class Iter1, class Iter2>
    static constexpr bool equal(Iter1 const& lhs, Iter2 const& rhs)
        AC_DEDUCE_NOEXCEPT_AND_RETURN(lhs.equals(rhs))

    template <class Iter>
    static constexpr void advance(Iter& iter, std::iter_difference_t<Iter> n)
        AC_DEDUCE_NOEXCEPT_AND_RETURN(iter.advance(n))

    template <class Iter1, class Iter2>
    static constexpr auto distance_to(Iter1 const& from, Iter2 const& to)
        AC_DEDUCE_NOEXCEPT_DECLTYPE_AND_RETURN(from.distance_to(to))
};

} // namespace ac
