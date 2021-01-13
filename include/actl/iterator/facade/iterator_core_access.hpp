// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/traits/dependent.hpp>

namespace ac {

struct iterator_core_access {
    template <class Iter>
    static constexpr reference_t<Iter> dereference(const Iter& iter) {
        return iter.dereference();
    }

    template <class Iter>
    static constexpr void increment(Iter& iter) {
        iter.increment();
    }

    template <class Iter>
    static constexpr void decrement(Iter& iter) {
        iter.decrement();
    }

    template <class Iter1, class Iter2>
    static constexpr bool equal(const Iter1& lhs, const Iter2& rhs) {
        return lhs.equals(rhs);
    }

    template <class Iter>
    static constexpr void advance(Iter& iter, difference_type_t<Iter> n) {
        iter.advance(n);
    }

    template <class Iter1, class Iter2>
    static constexpr difference_type_t<Iter1> distance_to(const Iter1& src, const Iter2& dst) {
        return src.distance_to(dst);
    }
};

}  // namespace ac
