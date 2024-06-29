// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/numeric/math.hpp>

namespace ac {

/// Comparable square root with deferred call to sqrt.
template<class T>
class square_root {
public:
    constexpr explicit square_root(T const& value = {}) : sqr_{value} {}

    operator decltype(eval(sqrt(std::declval<T>())))() const {
        return eval(sqrt(sqr_));
    }

    friend constexpr T const& perform(Sqr, square_root const& x) {
        return x.sqr_;
    }

    friend constexpr auto perform(
        ComparisonOperation auto op,
        square_root const& lhs,
        square_root const& rhs
    ) {
        return op(lhs.sqr_, rhs.sqr_);
    }

private:
    T sqr_;
};

struct defer_sqrt {
    struct is_policy;
};

template<class T>
auto perform(Sqrt, defer_sqrt, T const& x) {
    return square_root{x};
}

} // namespace ac
