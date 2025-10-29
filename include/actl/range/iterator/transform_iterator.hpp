// Copyright 2019 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/functional/traits/return.hpp>
#include <actl/range/iterator/interface/iterator_adaptor.hpp>
#include <actl/utility/use_default.hpp>

namespace ac {

/// Iterator that applies given function after dereferencing the base iterator.
template<class Iter, class Fn, class Value = use_default>
class transform_iterator
    : public iterator_adaptor<transform_iterator<Iter, Fn, Value>, Iter> {
    using base_t = iterator_adaptor<transform_iterator<Iter, Fn, Value>, Iter>;

public:
    using value_type = deduce_t<Value, std::remove_cvref_t<return_t<Fn>>>;

    transform_iterator() = default;

    template<class... Ts>
    transform_iterator(Iter const& iter, Ts&&... args)
        : base_t{iter}, m_fn{std::forward<Ts>(args)...} {}

    auto operator*() const AC_DEDUCE_NOEXCEPT_AND_RETURN(m_fn(*this->base()))

private:
    Fn m_fn;
};

} // namespace ac
