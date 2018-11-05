/***************************************************************************************************
 * Range facade is designed to produce complete range interface from a class with just two methods
 * begin() and end() that return iterators.
 ***************************************************************************************************
 * Copyright 2017 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/assert.hpp>
#include <iterator>

namespace ac {

namespace detail {

template <class R, class It, class C = typename std::iterator_traits<It>::iterator_category>
class range_facade {
    using traits = std::iterator_traits<It>;

public:
    using value_type      = typename traits::value_type;
    using reference       = typename traits::reference;
    using pointer         = typename traits::pointer;
    using difference_type = typename traits::difference_type;
    using iterator        = It;
    using size_type       = int;

    bool empty() const { return first() == last(); }

    explicit operator bool() const { return !empty(); }

    typename std::iterator_traits<iterator>::reference front() const {
        ACTL_ASSERT(!empty());
        return *first();
    }

protected:
    const R& derived() const { return *static_cast<const R*>(this); }

    iterator first() const { return derived().begin(); }
    iterator last() const { return derived().end(); }
};

template <class R, class It>
class range_facade<R, It, std::bidirectional_iterator_tag>
    : public range_facade<R, It, std::forward_iterator_tag> {
public:
    typename range_facade<R, It, std::forward_iterator_tag>::reference back() const {
        ACTL_ASSERT(!this->empty());
        auto last = this->last();
        return *--last;
    }
};

template <class R, class It>
class range_facade<R, It, std::random_access_iterator_tag>
    : public range_facade<R, It, std::bidirectional_iterator_tag> {
    using base_t = range_facade<R, It, std::bidirectional_iterator_tag>;

public:
    using typename base_t::size_type;

    typename base_t::reference operator[](typename base_t::difference_type at) const {
        ACTL_ASSERT(0 <= at && static_cast<size_type>(at) < size());
        return this->first()[at];
    }

    size_type size() const { return static_cast<size_type>(this->last() - this->first()); }
};

}  // namespace detail

template <class Range, class Iterator>
class range_facade : public detail::range_facade<Range, Iterator> {};

}  // namespace ac
