// Copyright 2019 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/functional/callable_traits.hpp>
#include <actl/iterator/facade/iterator_adaptor.hpp>
#include <actl/utility/use_default.hpp>

namespace ac {

/// Iterator that applies given function after dereferencing the base iterator.
template <class Iter, class Fn, class Value = use_default>
class transform_iterator
    : public iterator_adaptor<transform_iterator<Iter, Fn, Value>, Iter>
{
    using base_t = iterator_adaptor<transform_iterator<Iter, Fn, Value>, Iter>;

    friend struct ac::iterator_core_access;

    auto dereference() const
    {
        return fn_(*this->base());
    }

    Fn fn_;

public:
    using value_type = deduce_t<Value, remove_cvref_t<return_type_t<Fn>>>;

    template <class... Ts>
    transform_iterator(Iter const& iter, Ts&&... args)
        : base_t{iter}, fn_{std::forward<Ts>(args)...}
    {}
};

} // namespace ac
