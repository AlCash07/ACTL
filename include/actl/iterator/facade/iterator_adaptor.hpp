// Iterator adaptor is designed to simplify creation of similar iterators.
// Adopted from boost: http://www.boost.org/doc/libs/1_65_1/libs/iterator/doc/iterator_adaptor.html
//
// Copyright 2017 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/iterator/facade/iterator_facade.hpp>
#include <actl/util/use_default.hpp>

namespace ac {

// clang-format off
template <
    class Derived,
    class Iterator,
    class Category  = use_default,
    class Value     = use_default,
    class Reference = use_default,
    class Distance  = use_default>
class iterator_adaptor;

namespace detail {

template <class Derived, class It, class C, class V, class R, class D>
struct iterator_adaptor_base {
    using type = iterator_facade<Derived, iterator_types<
        deduce_t<C, typename std::iterator_traits<It>::iterator_category>,
        deduce_t<V, value_type_t<It>>,
        deduce_t<R, reference_t<It>>,
        deduce_t<D, difference_t<It>>>>;
};
// clang-format on

}  // namespace detail

template <class Derived, class It, class C, class V, class R, class D>
class iterator_adaptor : public detail::iterator_adaptor_base<Derived, It, C, V, R, D>::type {
public:
    explicit constexpr iterator_adaptor(const It& it) : it_{it} {}

    constexpr const It& base() const {
        return it_;
    }

protected:
    constexpr It& base_ref() {
        return it_;
    }

private:
    friend struct ac::iterator_core_access;

    using base_t = typename detail::iterator_adaptor_base<Derived, It, C, V, R, D>::type;

    constexpr reference_t<base_t> dereference() const {
        return *it_;
    }

    constexpr void increment() {
        ++it_;
    }

    constexpr void decrement() {
        --it_;
    }

    template <class T = difference_t<base_t>>
    constexpr void advance(T n) {
        it_ += n;
    }

    template <class Derived1, class It1, class C1, class V1, class R1, class D1>
    constexpr bool equals(const iterator_adaptor<Derived1, It1, C1, V1, R1, D1>& rhs) const {
        return it_ == rhs.base();
    }

    template <class Derived1, class It1, class C1, class V1, class R1, class D1>
    constexpr difference_t<base_t> distance_to(
        const iterator_adaptor<Derived1, It1, C1, V1, R1, D1>& rhs) const {
        return rhs.base() - it_;
    }

    It it_;
};

}  // namespace ac
