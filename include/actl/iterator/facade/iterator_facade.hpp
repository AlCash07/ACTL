// Iterator facade is designed to produce complete iterator interface from a set of core functions.
// Adopted from boost (with simplifications):
// http://www.boost.org/doc/libs/1_65_1/libs/iterator/doc/iterator_facade.html
//
// Copyright 2017 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/iterator/facade/iterator_core_access.hpp>
#include <actl/iterator/facade/iterator_types.hpp>
#include <actl/iterator/facade/operator_arrow_dispatch.hpp>
#include <actl/utility/operators.hpp>

namespace ac {

namespace detail {

template <class Iter, class T, class C>
class iter_facade : public operators::base<> {
public:
    using iterator_category = typename T::iterator_category;
    using value_type = std::remove_cv_t<typename T::value_type>;
    using reference = deduce_t<typename T::reference, value_type&>;
    using pointer = typename operator_arrow_dispatch<reference>::type;
    using difference_type = deduce_t<typename T::difference_type, std::ptrdiff_t>;

    // We don't return reference here because it's void for output iterators.
    constexpr decltype(auto) operator*() const {
        return iterator_core_access::dereference(derived());
    }

    constexpr Iter& operator++() {
        iterator_core_access::increment(derived());
        return derived();
    }

    constexpr Iter operator++(int) {
        Iter copy = derived();
        ++*this;
        return copy;
    }

protected:
    constexpr Iter& derived() {
        return static_cast<Iter&>(*this);
    }
    constexpr const Iter& derived() const {
        return static_cast<const Iter&>(*this);
    }
};

template <class Iter, class T>
class iter_facade<Iter, T, std::input_iterator_tag>
    : public iter_facade<Iter, T, std::output_iterator_tag> {
    using base_t = iter_facade<Iter, T, std::output_iterator_tag>;

public:
    constexpr typename base_t::pointer operator->() const {
        return operator_arrow_dispatch<typename base_t::reference>::apply(*this->derived());
    }
};

template <class Iter, class T>
class iter_facade<Iter, T, std::forward_iterator_tag>
    : public iter_facade<Iter, T, std::input_iterator_tag> {};

template <class Iter, class T>
class iter_facade<Iter, T, std::bidirectional_iterator_tag>
    : public iter_facade<Iter, T, std::forward_iterator_tag> {
public:
    constexpr Iter& operator--() {
        iterator_core_access::decrement(this->derived());
        return this->derived();
    }

    constexpr Iter operator--(int) {
        Iter copy = this->derived();
        --*this;
        return copy;
    }
};

template <class Iter, class T>
class iter_facade<Iter, T, std::random_access_iterator_tag>
    : public iter_facade<Iter, T, std::bidirectional_iterator_tag> {
    using base_t = iter_facade<Iter, T, std::bidirectional_iterator_tag>;
    using D = typename base_t::difference_type;

public:
    constexpr typename base_t::reference operator[](D n) const {
        return *(this->derived() + n);
    }

    constexpr Iter& operator+=(D n) {
        iterator_core_access::advance(this->derived(), n);
        return this->derived();
    }

    constexpr Iter& operator-=(D n) {
        return (*this) += (-n);
    }

    constexpr Iter operator+(D n) const {
        Iter copy = this->derived();
        return copy += n;
    }

    constexpr Iter operator-(D n) const {
        return (*this) + (-n);
    }
};

}  // namespace detail

template <class Derived, class Types>
class iterator_facade
    : public detail::iter_facade<Derived, Types, typename Types::iterator_category> {};

#define ITERATOR_OPERATOR(type, op, expr)                             \
    template <class Iter, class T>                                    \
    constexpr type operator op(const iterator_facade<Iter, T>& lhs,   \
                               const iterator_facade<Iter, T>& rhs) { \
        return expr;                                                  \
    }

ITERATOR_OPERATOR(bool, ==,
                  iterator_core_access::equal(static_cast<const Iter&>(lhs),
                                              static_cast<const Iter&>(rhs)))

ITERATOR_OPERATOR(auto, -,
                  iterator_core_access::distance_to(static_cast<const Iter&>(rhs),
                                                    static_cast<const Iter&>(lhs)))

ITERATOR_OPERATOR(bool, <, lhs - rhs < 0)

#undef ITERATOR_OPERATOR

template <class Iter, class T>
constexpr Iter operator+(typename T::difference_type n, const iterator_facade<Iter, T>& rhs) {
    return rhs + n;
}

}  // namespace ac
