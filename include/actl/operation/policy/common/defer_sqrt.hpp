// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/numeric/arithmetic/exponential/all.hpp>
#include <actl/operation/scalar/scalar_operation.hpp>

namespace ac {

/// Comparable square root with deferred call to sqrt.
template<typename T>
class deferred_square_root {
public:
    constexpr explicit deferred_square_root(T const& value = {})
        : m_squared{value} {}

    operator decltype(eval(square_root(m_squared)))() const {
        return eval(square_root(m_squared));
    }

    friend constexpr T const& perform(Square, deferred_square_root const& x) {
        return x.m_squared;
    }

    friend constexpr auto perform(
        ComparisonOperation auto op,
        deferred_square_root const& l,
        deferred_square_root const& r
    ) {
        return op(l.m_squared, r.m_squared);
    }

private:
    T m_squared;
};

struct defer_square_root {
    struct is_policy;
};

template<typename T>
auto perform(SquareRoot, defer_square_root, T const& x) {
    return deferred_square_root{x};
}

} // namespace ac
