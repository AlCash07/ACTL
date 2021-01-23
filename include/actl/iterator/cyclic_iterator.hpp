// Copyright 2017 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/assert.hpp>
#include <actl/iterator/facade/iterator_adaptor.hpp>
#include <actl/numeric/math.hpp>

namespace ac {

/// Iterator that moves from the last element to the first and vice versa.
template <class Range>
class cyclic_iterator : public iterator_adaptor<cyclic_iterator<Range>, iterator_t<Range>> {
    using Iter = iterator_t<Range>;

    Iter& iter() {
        return this->base_ref();
    }

public:
    explicit cyclic_iterator(Range& range, Iter iter)
        : iterator_adaptor<cyclic_iterator<Range>, Iter>{iter}, range_{&range} {
        ACTL_ASSERT(!std::empty(range));
        if (iter == end())
            this->base_ref() = begin();
    }

private:
    friend struct ac::iterator_core_access;

    Iter begin() const {
        return std::begin(*range_);
    }
    Iter end() const {
        return std::end(*range_);
    }

    void increment() {
        ++iter();
        if (iter() == end())
            iter() = begin();
    }

    void decrement() {
        if (iter() == begin())
            iter() = end();
        --iter();
    }

    void advance(difference_type_t<Iter> n) {
        auto cycle = static_cast<difference_type_t<Iter>>(std::size(*range_));
        ACTL_ASSERT(abs(n) < cycle);
        if (n > 0) {
            iter() += n - (n >= (end() - iter()) ? cycle : 0);
        } else {
            iter() += n + (-n > (iter() - begin()) ? cycle : 0);
        }
    }

    difference_type_t<Iter> distance_to(const cyclic_iterator& rhs) const {
        auto dist = rhs.base() - this->base();
        return dist >= 0 ? dist : dist + static_cast<difference_type_t<Iter>>(std::size(*range_));
    }

    Range* range_;
};

template <class Range>
auto cyclic_begin(Range& range) {
    return cyclic_iterator{range, std::begin(range)};
}

}  // namespace ac
