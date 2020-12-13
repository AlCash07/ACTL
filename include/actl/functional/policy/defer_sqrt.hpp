// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/functional/scalar/math.hpp>

namespace ac::math {

/**
 * Comparable square root with deferred call to sqrt.
 */
template <class T>
class square_root {
public:
    constexpr explicit square_root(const T& value = {}) : sqr_{value} {}

    operator decltype(eval(sqrt(std::declval<T>())))() const { return eval(sqrt(sqr_)); }

    friend constexpr const T& perform(Sqr, const square_root& x) { return x.sqr_; }

    template <class Op, enable_int_if<is_comparison_v<Op>> = 0>
    friend constexpr auto perform(Op op, const square_root& lhs, const square_root& rhs) {
        return op(lhs.sqr_, rhs.sqr_);
    }

private:
    T sqr_;
};

struct defer_sqrt {
    struct is_policy;
};

template <class T>
auto perform(Sqrt, defer_sqrt, const T& x) {
    return square_root{x};
}

}  // namespace ac::math
