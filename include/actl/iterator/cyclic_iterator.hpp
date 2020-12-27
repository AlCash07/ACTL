// Copyright 2017 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/assert.hpp>
#include <actl/functional/scalar/math.hpp>
#include <actl/iterator/facade/iterator_adaptor.hpp>

namespace ac {

/**
 * Iterator that moves from the last element to the first and vice versa.
 */
template <class Range>
class cyclic_iterator : public iterator_adaptor<cyclic_iterator<Range>, iterator_t<Range>> {
    using It = iterator_t<Range>;

    It& it() {
        return this->base_ref();
    }

public:
    explicit cyclic_iterator(Range& range, It it)
        : iterator_adaptor<cyclic_iterator<Range>, It>{it}, range_{&range} {
        ACTL_ASSERT(!std::empty(range));
        if (it == end())
            this->base_ref() = begin();
    }

private:
    friend struct ac::iterator_core_access;

    It begin() const {
        return std::begin(*range_);
    }
    It end() const {
        return std::end(*range_);
    }

    void increment() {
        ++it();
        if (it() == end())
            it() = begin();
    }

    void decrement() {
        if (it() == begin())
            it() = end();
        --it();
    }

    void advance(difference_t<It> n) {
        auto cycle = static_cast<difference_t<It>>(std::size(*range_));
        ACTL_ASSERT(abs(n) < cycle);
        if (n > 0) {
            it() += n - (n >= (end() - it()) ? cycle : 0);
        } else {
            it() += n + (-n > (it() - begin()) ? cycle : 0);
        }
    }

    difference_t<It> distance_to(const cyclic_iterator& rhs) const {
        auto dist = rhs.base() - this->base();
        return dist >= 0 ? dist : dist + static_cast<difference_t<It>>(std::size(*range_));
    }

    Range* range_;
};

template <class Range>
auto cyclic_begin(Range& range) {
    return cyclic_iterator{range, std::begin(range)};
}

}  // namespace ac
