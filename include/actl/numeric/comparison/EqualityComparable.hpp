// Copyright 2023 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <concepts>

namespace ac {

template<typename T>
concept NothrowEqualityComparable =
    std::equality_comparable<T> && requires(const T& l, const T& r) {
        { l == r } noexcept;
        { l != r } noexcept;
    };

// TODO: add NothrowEqualityComparableWith.

} // namespace ac
