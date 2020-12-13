// Copyright 2019 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/functional/traits.hpp>
#include <actl/iterator/facade/iterator_adaptor.hpp>

namespace ac {

/**
 * Iterator that applies given function after dereferencing base iterator.
 */
template <class Iterator, class Function, class Value = use_default>
class transform_iterator;

namespace detail {

template <class It, class F, class V>
using transform_it_base = iterator_adaptor<transform_iterator<It, F, V>, It, use_default,
                                           return_t<F>, deduce_t<V, remove_cvref_t<return_t<F>>>>;

}  // namespace detail

template <class It, class F, class V>
class transform_iterator : public detail::transform_it_base<It, F, V> {
    friend struct ac::iterator_core_access;

    auto dereference() const { return f_(*this->base()); }

    F f_;

public:
    template <class... Ts>
    transform_iterator(const It& it, Ts&&... args)
        : detail::transform_it_base<It, F, V>{it}, f_{std::forward<Ts>(args)...} {}
};

}  // namespace ac
