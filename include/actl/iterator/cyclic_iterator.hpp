/***************************************************************************************************
 * Copyright 2017 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/assert.hpp>
#include <actl/iterator/iterator_adaptor.hpp>
#include <actl/numeric/math.hpp>

namespace ac {

/**
 * Iterator that moves from the last element to the first and vice versa.
 */
template <class Range>
class cyclic_iterator
    : public iterator_adaptor<cyclic_iterator<Range>, iterator_t<std::remove_reference_t<Range>>> {
    using It = iterator_t<std::remove_reference_t<Range>>;

    It& it() { return this->base_ref(); }

public:
    explicit cyclic_iterator(It it, Range range)
        : iterator_adaptor<cyclic_iterator<Range>, It>{it}, range_{std::forward<Range>(range)} {
        ACTL_ASSERT(!std::empty(range));
        if (it == end()) it() = begin();
    }

private:
    friend struct ac::iterator_core_access;

    It begin() const { return std::begin(range_); }
    It end() const { return std::end(range_); }

    void increment() {
        ++it();
        if (it() == end()) it() = begin();
    }

    void decrement() {
        if (it() == begin()) it() = end();
        --it();
    }

    void advance(difference_t<It> n) {
        auto cycle = std::size(range_);
        ACTL_ASSERT(adl::abs(n) < cycle);
        if (n > 0) {
            it() += n - (n >= (end() - it()) ? cycle : 0);
        } else {
            it() += n + (-n > (it() - begin()) ? cycle : 0);
        }
    }

    difference_t<It> distance_to(const cyclic_iterator& rhs) const {
        auto distance = rhs.base() - this->base();
        return distance >= 0 ? distance : distance + std::size(range_);
    }

    Range range_;
};

}  // namespace ac
