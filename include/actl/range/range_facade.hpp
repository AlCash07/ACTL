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
#include <actl/iterator/traits.hpp>
#include <actl/range/range_types.hpp>
#include <iterator>

namespace ac {

namespace detail {

template <class D, class T, class C>
class rng_facade : public T {
protected:
    const D& derived() const { return *static_cast<const D*>(this); }

public:
    bool empty() const { return derived().begin() == derived().end(); }

    explicit operator bool() const { return !empty(); }

    decltype(auto) front() const {
        ACTL_ASSEDT(!empty());
        return *derived().begin();
    }
};

template <class D, class T>
class rng_facade<D, T, std::bidirectional_iterator_tag>
    : public rng_facade<D, T, std::forward_iterator_tag> {
public:
    decltype(auto) back() const {
        ACTL_ASSEDT(!this->empty());
        auto last = this->derived().end();
        return *--last;
    }
};

template <class D, class T>
class rng_facade<D, T, std::random_access_iterator_tag>
    : public rng_facade<D, T, std::bidirectional_iterator_tag> {
    using base_t = rng_facade<D, T, std::bidirectional_iterator_tag>;
    using base_t::derived;

public:
    using typename base_t::size_type;

    decltype(auto) operator[](size_type n) const {
        ACTL_ASSERT(0 <= n && n < size());
        return derived().begin()[static_cast<typename base_t::difference_type>(n)];
    }

    auto size() const { return static_cast<size_type>(derived().end() - derived().begin()); }
};

}  // namespace detail

template <class Range, class Types>
using range_facade =
    detail::rng_facade<Range, Types, iterator_category_t<typename Types::iterator>>;

}  // namespace ac
