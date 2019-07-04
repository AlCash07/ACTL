/***************************************************************************************************
 * Iterator facade is designed to produce complete iterator interface from a set of core functions.
 * Adopted from boost (with simplifications):
 * http://www.boost.org/doc/libs/1_65_1/libs/iterator/doc/iterator_facade.html
 ***************************************************************************************************
 * Copyright 2017 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/util/operators.hpp>
#include <actl/util/type_traits.hpp>
#include <iterator>
#include <memory>

namespace ac {

template <class Derived,
          class Category,
          class Value,
          class Reference = Value&,
          class Pointer   = Value*,
          class Distance  = std::ptrdiff_t>
class iterator_facade;

struct iterator_core_access {
    template <class It>
    static reference_t<It> dereference(const It& it) {
        return it.dereference();
    }

    template <class It>
    static void increment(It& it) { it.increment(); }

    template <class It>
    static void decrement(It& it) { it.decrement(); }

    template <class It1, class It2>
    static bool equal(const It1& it1, const It2& it2) { return it1.equals(it2); }

    template <class It>
    static void advance(It& it, difference_t<It> n) {
        it.advance(n);
    }

    template <class It1, class It2>
    static difference_t<It1> distance_to(const It1& it1, const It2& it2) {
        return it1.distance_to(it2);
    }
};

namespace detail {

template <class Ref, class Pointer>
struct operator_arrow_dispatch {  // proxy references
    struct proxy {
        explicit proxy(const Ref& ref) : ref_{ref} {}
        Ref* operator->() { return std::addressof(ref_); }
        Ref ref_;
    };

    using type = proxy;

    static type apply(const Ref& x) { return type(x); }
};

template <class T, class Pointer>
struct operator_arrow_dispatch<T&, Pointer> {  // "real" references
    using type = Pointer;

    static type apply(T& x) { return std::addressof(x); }
};

template <class Iterator, class Category, class Value, class Reference, class Pointer,
          class Distance>
class it_facade;

template <class It, class V, class R, class P, class D>
class it_facade<It, std::output_iterator_tag, V, R, P, D> : operators::base<> {
public:
	using iterator_category = std::output_iterator_tag;
	using value_type        = V;
	using reference         = R;
	using pointer           = typename operator_arrow_dispatch<R, P>::type;
	using difference_type   = D;

    R operator*() const { return iterator_core_access::dereference(derived()); }

    It& operator++() {
        iterator_core_access::increment(derived());
        return derived();
    }

protected:
    It& derived() { return *static_cast<It*>(this); }

    const It& derived() const { return *static_cast<const It*>(this); }
};

template <class It, class V, class R, class P, class D>
class it_facade<It, std::input_iterator_tag, V, R, P, D>
    : public it_facade<It, std::output_iterator_tag, V, R, P, D> {
public:
    using typename it_facade<It, std::output_iterator_tag, V, R, P, D>::pointer;

    pointer operator->() const { return operator_arrow_dispatch<R, P>::apply(*this->derived()); }
};

template <class It, class V, class R, class P, class D>
class it_facade<It, std::forward_iterator_tag, V, R, P, D>
    : public it_facade<It, std::input_iterator_tag, V, R, P, D> {};

template <class It, class V, class R, class P, class D>
class it_facade<It, std::bidirectional_iterator_tag, V, R, P, D>
    : public it_facade<It, std::forward_iterator_tag, V, R, P, D> {
public:
    It& operator--() {
        iterator_core_access::decrement(this->derived());
        return this->derived();
    }
};

template <class It, class V, class R, class P, class D>
class it_facade<It, std::random_access_iterator_tag, V, R, P, D>
    : public it_facade<It, std::bidirectional_iterator_tag, V, R, P, D> {
public:
    R operator[](D n) const { return *(this->derived() + n); }

    It& operator += (D n) {
        iterator_core_access::advance(this->derived(), n);
        return this->derived();
    }

    It& operator -= (D n) { return (*this) += (-n); }

    It operator + (D n) const {
        It copy = this->derived();
        return copy += n;
    }

    It operator - (D n) const { return (*this) + (-n); }
};

}  // namespace detail

template <class It, class C, class V, class R, class P, class D>
class iterator_facade : public detail::it_facade<It, C, V, R, P, D> {
public:
    using iterator_category = C;
};

#define ITERATOR_OPERATOR(type, op, expr)                                    \
    template <class It, class C, class V, class R, class P, class D>         \
    inline type operator op(const iterator_facade<It, C, V, R, P, D>& lhs,   \
                            const iterator_facade<It, C, V, R, P, D>& rhs) { \
        return expr;                                                         \
    }

ITERATOR_OPERATOR(bool, ==,
                  iterator_core_access::equal(*static_cast<const It*>(&lhs),
                                              *static_cast<const It*>(&rhs)))

ITERATOR_OPERATOR(D, -,
                  iterator_core_access::distance_to(*static_cast<const It*>(&rhs),
                                                    *static_cast<const It*>(&lhs)))

ITERATOR_OPERATOR(bool, <, lhs - rhs < 0)

template <class It, class C, class V, class R, class P, class D>
inline It operator + (D n, const iterator_facade<It, C, V, R, P, D>& rhs) { return rhs + n; }

#undef ITERATOR_OPERATOR

}  // namespace ac
