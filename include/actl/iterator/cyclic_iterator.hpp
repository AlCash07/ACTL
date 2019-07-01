/***************************************************************************************************
 * Copyright 2017 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/assert.hpp>
#include <actl/iterator/iterator_facade.hpp>
#include <actl/macros.hpp>
#include <actl/numeric/functions.hpp>

namespace ac {

/**
 * Iterator that moves from the last element to the first and vice versa.
 */
template <class Iterator>
class cyclic_iterator
    : public iterator_facade<cyclic_iterator<Iterator>,
                             typename std::iterator_traits<Iterator>::iterator_category,
                             value_t<Iterator>,
                             reference_t<Iterator>,
                             pointer_t<Iterator>,
                             difference_t<Iterator>> {
public:
    cyclic_iterator(Iterator it, Iterator begin, Iterator end)
        : it_{it != end ? it : begin}, begin_{begin}, end_{end} {}

private:
    friend struct ac::iterator_core_access;

    reference_t<Iterator> dereference() const { return *it_; }

    void increment() {
        ++it_;
        if (EXPECT_FALSE(it_ == end_)) it_ = begin_;
    }

    void decrement() {
        if (EXPECT_FALSE(it_ == begin_)) it_ = end_;
        --it_;
    }

    void advance(difference_t<Iterator> n) {
        auto cycle = end_ - begin_;
        ACTL_ASSERT(abs(n) < cycle);
        if (n > 0) {
            it_ += n - (n >= end_ - it_ ? cycle : 0);
        } else {
            it_ += n + (-n > it_ - begin_ ? cycle : 0);
        }
    }

    bool equals(const cyclic_iterator& rhs) const { return it_ == rhs.it_; }

    difference_t<Iterator> distance_to(const cyclic_iterator& rhs) const {
        auto distance = rhs.it_ - it_;
        return distance >= 0 ? distance : distance + (end_ - begin_);
    }

    Iterator it_;
    Iterator begin_;
    Iterator end_;
};

}  // namespace ac
