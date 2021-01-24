// Iterator adaptor is designed to simplify creation of similar iterators.
// Adopted from boost: http://www.boost.org/doc/libs/1_65_1/libs/iterator/doc/iterator_adaptor.html
//
// Copyright 2017 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/iterator/facade/iterator_facade.hpp>
#include <actl/iterator/traits.hpp>

namespace ac {

namespace detail {

template <class Iter, class T>
struct deduced_iter_types {
    using iterator_category = deduce_t<typename T::iterator_category, iterator_category_t<Iter>>;
    using value_type = deduce_t<typename T::value_type, value_type_t<Iter>>;
    using reference = deduce_t<typename T::reference, reference_t<Iter>>;
    using difference_type = deduce_t<typename T::difference_type, difference_type_t<Iter>>;
};

} // namespace detail

template <class Derived, class Iter, class Types = void>
class iterator_adaptor : public iterator_facade<Derived, detail::deduced_iter_types<Iter, Types>> {
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

    using base_t = iterator_facade<Derived, detail::deduced_iter_types<Iter, Types>>;

    constexpr reference_t<base_t> dereference() const {
        return *iter_;
    }

    constexpr void increment() {
        ++iter_;
    }

    constexpr void decrement() {
        --iter_;
    }

    template <class D = difference_type_t<base_t>>
    constexpr void advance(D n) {
        iter_ += n;
    }

    template <class Derived1, class Iter1, class Types1>
    constexpr bool equals(const iterator_adaptor<Derived1, Iter1, Types1>& rhs) const {
        return iter_ == rhs.base();
    }

    template <class Derived1, class Iter1, class Types1>
    constexpr difference_type_t<base_t> distance_to(
        const iterator_adaptor<Derived1, Iter1, Types1>& rhs) const {
        return rhs.base() - iter_;
    }

    Iter iter_;
};

} // namespace ac
