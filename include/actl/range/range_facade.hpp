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

#define ENABLE_NON_CONST template <class U = T, class = typename U::const_iterator>

template <class D, class T, class C>
class rng_facade : public T {
public:
    ENABLE_NON_CONST bool empty() { return derived().begin() == derived().end(); }
    bool empty() const { return derived().begin() == derived().end(); }

    ENABLE_NON_CONST explicit operator bool() { return !empty(); }
    explicit operator bool() const { return !empty(); }

    ENABLE_NON_CONST decltype(auto) front() {
        ACTL_ASSERT(!empty());
        return *derived().begin();
    }

    decltype(auto) front() const {
        ACTL_ASSERT(!empty());
        return *derived().begin();
    }

protected:
    D& derived() { return static_cast<D&>(*this); }
    const D& derived() const { return static_cast<const D&>(*this); }
};

template <class D, class T>
class rng_facade<D, T, std::bidirectional_iterator_tag>
    : public rng_facade<D, T, std::forward_iterator_tag> {
public:
    ENABLE_NON_CONST decltype(auto) back() {
        ACTL_ASSERT(!this->empty());
        auto last = this->derived().end();
        return *--last;
    }

    decltype(auto) back() const {
        ACTL_ASSERT(!this->empty());
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

    ENABLE_NON_CONST decltype(auto) operator[](size_type n) {
        ACTL_ASSERT(0 <= n && n < size());
        return derived().begin()[static_cast<typename base_t::difference_type>(n)];
    }

    decltype(auto) operator[](size_type n) const {
        ACTL_ASSERT(0 <= n && n < size());
        return derived().begin()[static_cast<typename base_t::difference_type>(n)];
    }

    ENABLE_NON_CONST auto size() {
        return static_cast<size_type>(derived().end() - derived().begin());
    }
    auto size() const { return static_cast<size_type>(derived().end() - derived().begin()); }
};

#undef ENABLE_NON_CONST

}  // namespace detail

template <class Range, class Types>
using range_facade =
    detail::rng_facade<Range, Types, iterator_category_t<typename Types::iterator>>;

}  // namespace ac
