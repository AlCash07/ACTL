// Copyright 2023 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <concepts>

namespace ac {

template <class T>
concept NothrowEqualityComparable =
    std::equality_comparable<T> && requires(const T& t, const T& u) {
                                       {
                                           t == u
                                       } noexcept;
                                       {
                                           t != u
                                       } noexcept;
                                   };

// TODO: add NothrowEqualityComparableWith.

} // namespace ac
