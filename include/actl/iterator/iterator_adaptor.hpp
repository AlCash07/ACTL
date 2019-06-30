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
#include <actl/util/use_default.hpp>

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
    using traits = std::iterator_traits<It>;

    using type = iterator_facade<
        Derived,
        deduce_t<C, typename traits::iterator_category>,
        deduce_t<V, typename traits::value_type>,
        deduce_t<R, typename traits::reference>,
        deduce_t<P, typename traits::pointer>,
        deduce_t<D, typename traits::difference_type>>;
};

}  // namespace detail

template <class Derived, class It, class C, class V, class R, class P, class D>
class iterator_adaptor : public detail::iterator_adaptor_base<Derived, It, C, V, R, P, D>::type {
public:
    explicit iterator_adaptor(const It& it) : it_{it} {}

    const It& base() const { return it_; }

protected:
    It& base_ref() { return it_; }

private:
    friend struct iterator_core_access;

    using base_t = typename detail::iterator_adaptor_base<Derived, It, C, V, R, P, D>::type;

    typename base_t::reference dereference() const { return *it_; }

    void increment() { ++it_; }

    void decrement() { --it_; }

    template <class T = typename base_t::difference_type>
    void advance(T n) { it_ += n; }

    template <class Derived1, class It1, class C1, class V1, class R1, class P1, class D1>
    bool equals(const iterator_adaptor<Derived1, It1, C1, V1, R1, P1, D1>& rhs) const {
        return it_ == rhs.base();
    }

    template <class Derived1, class It1, class C1, class V1, class R1, class P1, class D1>
    typename base_t::difference_type distance_to(
        const iterator_adaptor<Derived1, It1, C1, V1, R1, P1, D1>& rhs) const {
        return rhs.base() - it_;
    }

    It it_;
};

}  // namespace ac
