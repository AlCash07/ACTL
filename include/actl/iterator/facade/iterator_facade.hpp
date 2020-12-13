// Iterator facade is designed to produce complete iterator interface from a set of core functions.
// Adopted from boost (with simplifications):
// http://www.boost.org/doc/libs/1_65_1/libs/iterator/doc/iterator_facade.html
//
// Copyright 2017 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/functional/operators.hpp>
#include <actl/iterator/facade/iterator_types.hpp>
#include <actl/util/type_traits.hpp>

namespace ac {

struct iterator_core_access {
    template <class It>
    static constexpr reference_t<It> dereference(const It& it) {
        return it.dereference();
    }

    template <class It>
    static constexpr void increment(It& it) {
        it.increment();
    }

    template <class It>
    static constexpr void decrement(It& it) {
        it.decrement();
    }

    template <class It1, class It2>
    static constexpr bool equal(const It1& it1, const It2& it2) {
        return it1.equals(it2);
    }

    template <class It>
    static constexpr void advance(It& it, difference_t<It> n) {
        it.advance(n);
    }

    template <class It1, class It2>
    static constexpr difference_t<It1> distance_to(const It1& it1, const It2& it2) {
        return it1.distance_to(it2);
    }
};

namespace detail {

template <class It, class T, class C>
class it_facade : public operators::base<T> {
public:
    // We don't return reference here because it's void for output iterators.
    constexpr decltype(auto) operator*() const {
        return iterator_core_access::dereference(derived());
    }

    constexpr It& operator++() {
        iterator_core_access::increment(derived());
        return derived();
    }

    constexpr It operator++(int) {
        It copy = derived();
        ++*this;
        return copy;
    }

protected:
    constexpr It& derived() { return static_cast<It&>(*this); }
    constexpr const It& derived() const { return static_cast<const It&>(*this); }
};

template <class It, class T>
class it_facade<It, T, std::input_iterator_tag>
    : public it_facade<It, T, std::output_iterator_tag> {
    using base_t = it_facade<It, T, std::output_iterator_tag>;

public:
    constexpr typename base_t::pointer operator->() const {
        return operator_arrow_dispatch<typename base_t::reference>::apply(*this->derived());
    }
};

template <class It, class T>
class it_facade<It, T, std::forward_iterator_tag>
    : public it_facade<It, T, std::input_iterator_tag> {};

template <class It, class T>
class it_facade<It, T, std::bidirectional_iterator_tag>
    : public it_facade<It, T, std::forward_iterator_tag> {
public:
    constexpr It& operator--() {
        iterator_core_access::decrement(this->derived());
        return this->derived();
    }

    constexpr It operator--(int) {
        It copy = this->derived();
        --*this;
        return copy;
    }
};

template <class It, class T>
class it_facade<It, T, std::random_access_iterator_tag>
    : public it_facade<It, T, std::bidirectional_iterator_tag> {
    using base_t = it_facade<It, T, std::bidirectional_iterator_tag>;
    using D = typename base_t::difference_type;

public:
    constexpr typename base_t::reference operator[](D n) const { return *(this->derived() + n); }

    constexpr It& operator += (D n) {
        iterator_core_access::advance(this->derived(), n);
        return this->derived();
    }

    constexpr It& operator -= (D n) { return (*this) += (-n); }

    constexpr It operator + (D n) const {
        It copy = this->derived();
        return copy += n;
    }

    constexpr It operator - (D n) const { return (*this) + (-n); }
};

}  // namespace detail

template <class Derived, class Types>
class iterator_facade
    : public detail::it_facade<Derived, Types, typename Types::iterator_category> {};

#define ITERATOR_OPERATOR(type, op, expr)                           \
    template <class It, class T>                                    \
    constexpr type operator op(const iterator_facade<It, T>& lhs,   \
                               const iterator_facade<It, T>& rhs) { \
        return expr;                                                \
    }

ITERATOR_OPERATOR(bool, ==,
                  iterator_core_access::equal(*static_cast<const It*>(&lhs),
                                              *static_cast<const It*>(&rhs)))

ITERATOR_OPERATOR(auto, -,
                  iterator_core_access::distance_to(*static_cast<const It*>(&rhs),
                                                    *static_cast<const It*>(&lhs)))

ITERATOR_OPERATOR(bool, <, lhs - rhs < 0)

#undef ITERATOR_OPERATOR

template <class It, class T>
constexpr It operator + (typename T::difference_type n, const iterator_facade<It, T>& rhs) {
    return rhs + n;
}

}  // namespace ac
