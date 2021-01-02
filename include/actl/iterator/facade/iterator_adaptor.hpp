// Iterator adaptor is designed to simplify creation of similar iterators.
// Adopted from boost: http://www.boost.org/doc/libs/1_65_1/libs/iterator/doc/iterator_adaptor.html
//
// Copyright 2017 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/iterator/facade/iterator_facade.hpp>
#include <actl/utility/use_default.hpp>

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

template <class Derived, class Iter, class C, class V, class R, class D>
struct iterator_adaptor_base {
    using type = iterator_facade<Derived, iterator_types<
        deduce_t<C, typename std::iterator_traits<Iter>::iterator_category>,
        deduce_t<V, value_type_t<Iter>>,
        deduce_t<R, reference_t<Iter>>,
        deduce_t<D, difference_t<Iter>>>>;
};
// clang-format on

}  // namespace detail

template <class Derived, class Iter, class C, class V, class R, class D>
class iterator_adaptor : public detail::iterator_adaptor_base<Derived, Iter, C, V, R, D>::type {
public:
    explicit constexpr iterator_adaptor(const Iter& iter) : iter_{iter} {}

    constexpr const Iter& base() const {
        return iter_;
    }

protected:
    constexpr Iter& base_ref() {
        return iter_;
    }

private:
    friend struct ac::iterator_core_access;

    using base_t = typename detail::iterator_adaptor_base<Derived, Iter, C, V, R, D>::type;

    constexpr reference_t<base_t> dereference() const {
        return *iter_;
    }

    constexpr void increment() {
        ++iter_;
    }

    constexpr void decrement() {
        --iter_;
    }

    template <class T = difference_t<base_t>>
    constexpr void advance(T n) {
        iter_ += n;
    }

    template <class Derived1, class Iter1, class C1, class V1, class R1, class D1>
    constexpr bool equals(const iterator_adaptor<Derived1, Iter1, C1, V1, R1, D1>& rhs) const {
        return iter_ == rhs.base();
    }

    template <class Derived1, class Iter1, class C1, class V1, class R1, class D1>
    constexpr difference_t<base_t> distance_to(
        const iterator_adaptor<Derived1, Iter1, C1, V1, R1, D1>& rhs) const {
        return rhs.base() - iter_;
    }

    Iter iter_;
};

}  // namespace ac
