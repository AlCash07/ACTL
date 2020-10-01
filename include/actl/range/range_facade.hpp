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
    ENABLE_NON_CONST constexpr auto cbegin() const { return derived().begin(); }
    ENABLE_NON_CONST constexpr auto cend() const { return derived().end(); }

    constexpr bool empty() const { return derived().begin() == derived().end(); }
    ENABLE_NON_CONST constexpr bool empty() { return derived().begin() == derived().end(); }

    explicit constexpr operator bool() const { return !empty(); }
    ENABLE_NON_CONST explicit constexpr operator bool() { return !empty(); }

    constexpr decltype(auto) front() const {
        ACTL_ASSERT(!empty());
        return *derived().begin();
    }

    ENABLE_NON_CONST constexpr decltype(auto) front() {
        ACTL_ASSERT(!empty());
        return *derived().begin();
    }

protected:
    constexpr const D& derived() const { return static_cast<const D&>(*this); }
    constexpr D& derived() { return static_cast<D&>(*this); }
};

template <class T, class = void>
struct crev_it {
    using type = typename T::reverse_iterator;
};

template <class T>
struct crev_it<T, std::void_t<typename T::const_reverse_iterator>> {
    using type = typename T::const_reverse_iterator;
};

template <class T>
using crev_it_t = typename crev_it<T>::type;

template <class D, class T>
class rng_facade<D, T, std::bidirectional_iterator_tag>
    : public rng_facade<D, T, std::forward_iterator_tag> {
public:
    constexpr auto rbegin() const { return crev_it_t<T>{this->derived().end()}; }
    ENABLE_NON_CONST constexpr auto rbegin() {
        return typename T::reverse_iterator{this->derived().end()};
    }
    ENABLE_NON_CONST constexpr auto crbegin() const { return rbegin(); }

    constexpr auto rend() const { return crev_it_t<T>{this->derived().begin()}; }
    ENABLE_NON_CONST constexpr auto rend() {
        return typename T::reverse_iterator{this->derived().begin()};
    }
    ENABLE_NON_CONST constexpr auto crend() const { return rend(); }

    constexpr decltype(auto) back() const {
        ACTL_ASSERT(!this->empty());
        auto last = this->derived().end();
        return *--last;
    }

    ENABLE_NON_CONST constexpr decltype(auto) back() {
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

    constexpr decltype(auto) operator[](size_type n) const {
        ACTL_ASSERT(0 <= n && n < size());
        return derived().begin()[static_cast<typename base_t::difference_type>(n)];
    }

    ENABLE_NON_CONST constexpr decltype(auto) operator[](size_type n) {
        ACTL_ASSERT(0 <= n && n < size());
        return derived().begin()[static_cast<typename base_t::difference_type>(n)];
    }

    constexpr auto size() const {
        return static_cast<size_type>(derived().end() - derived().begin());
    }

    ENABLE_NON_CONST constexpr auto size() {
        return static_cast<size_type>(derived().end() - derived().begin());
    }
};

#undef ENABLE_NON_CONST

}  // namespace detail

template <class Range, class Types>
using range_facade =
    detail::rng_facade<Range, Types, iterator_category_t<typename Types::iterator>>;

}  // namespace ac
