// Copyright 2019 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/iterator/facade/iterator_adaptor.hpp>
#include <actl/meta/function_traits.hpp>
#include <actl/utility/use_default.hpp>

namespace ac {

/// Iterator that applies given function after dereferencing base iterator.
template <class Iterator, class Function, class Value = use_default>
class transform_iterator;

namespace detail {

template <class F, class V>
struct transform_iter_types : default_iterator_adaptor_types {
    using value_type = return_type_t<F>;
    using reference = deduce_t<V, remove_cvref_t<return_type_t<F>>>;
};

template <class Iter, class F, class V>
using transform_iter_base = iterator_adaptor<
    transform_iterator<Iter, F, V>,
    Iter,
    transform_iter_types<F, V>>;

} // namespace detail

template <class Iter, class F, class V>
class transform_iterator : public detail::transform_iter_base<Iter, F, V> {
    friend struct ac::iterator_core_access;

    auto dereference() const {
        return f_(*this->base());
    }

    F f_;

public:
    template <class... Ts>
    transform_iterator(const Iter& iter, Ts&&... args)
        : detail::transform_iter_base<Iter, F, V>{iter}
        , f_{std::forward<Ts>(args)...} {}
};

} // namespace ac
