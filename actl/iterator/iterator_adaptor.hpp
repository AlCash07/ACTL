/***************************************************************************************************
 * Iterator adaptor is designed to simplify creation of similar iterators.
 * Adopted from boost: http://www.boost.org/doc/libs/1_65_1/libs/iterator/doc/iterator_adaptor.html
 ***************************************************************************************************
 * Copyright 2017 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/iterator/iterator_facade.hpp>
#include <actl/type/use_default.hpp>

namespace ac {

template <class Derived,
          class Iterator,
          class Category  = use_default,
          class Value     = use_default,
          class Reference = use_default,
          class Pointer   = use_default,
          class Distance  = use_default>
class iterator_adaptor;

namespace detail {

template <class Derived, class It, class C, class V, class R, class P, class D>
struct iterator_adaptor_base {
    using type =
        iterator_facade<Derived,
                        deduce_type_t<C, typename std::iterator_traits<It>::iterator_category>,
                        deduce_type_t<V, typename std::iterator_traits<It>::value_type>,
                        deduce_type_t<R, typename std::iterator_traits<It>::reference>,
                        deduce_type_t<P, typename std::iterator_traits<It>::pointer>,
                        deduce_type_t<D, typename std::iterator_traits<It>::difference_type>>;
};

}  // namespace detail

template <class Derived, class It, class C, class V, class R, class P, class D>
class iterator_adaptor : public detail::iterator_adaptor_base<Derived, It, C, V, R, P, D>::type {
public:
    explicit iterator_adaptor(const It& it) : it_{it} {}

    const It& base() const { return it_; }

protected:
    It& base() { return it_; }

private:
    using base_type = typename detail::iterator_adaptor_base<Derived, It, C, V, R, P, D>::type;

    friend struct iterator_core_access;

    typename base_type::reference dereference() const { return *it_; }

    void increment() { ++it_; }

    void decrement() { --it_; }

    template <class T = typename base_type::difference_type>
    void advance(T n) { it_ += n; }

    template <class Derived1, class It1, class C1, class V1, class R1, class P1, class D1>
    bool equals(const iterator_adaptor<Derived1, It1, C1, V1, R1, P1, D1>& other) const {
        return it_ == other.base();
    }

    template <class Derived1, class It1, class C1, class V1, class R1, class P1, class D1>
    typename base_type::difference_type distance_to(
        const iterator_adaptor<Derived1, It1, C1, V1, R1, P1, D1>& other) const {
        return other.base() - it_;
    }

    It it_;
};

}  // namespace ac
