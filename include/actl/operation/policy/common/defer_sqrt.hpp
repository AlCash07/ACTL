// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/numeric/math.hpp>

namespace ac {

/// Comparable square root with deferred call to sqrt.
template<typename T>
class square_root {
public:
    constexpr explicit square_root(T const& value = {}) : m_sqr{value} {}

    operator decltype(eval(sqrt(std::declval<T>())))() const {
        return eval(sqrt(m_sqr));
    }

    friend constexpr T const& perform(Sqr, square_root const& x) {
        return x.m_sqr;
    }

    friend constexpr auto perform(
        ComparisonOperation auto op, square_root const& l, square_root const& r
    ) {
        return op(l.m_sqr, r.m_sqr);
    }

private:
    T m_sqr;
};

struct defer_sqrt {
    struct is_policy;
};

template<typename T>
auto perform(Sqrt, defer_sqrt, T const& x) {
    return square_root{x};
}

} // namespace ac
