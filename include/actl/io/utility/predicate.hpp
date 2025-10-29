// Copyright 2019 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <cstring>
#include <type_traits>

namespace ac::io {

template<typename T>
struct predicate {
    T pred;

    template<typename Char>
    constexpr bool operator()(Char c) const {
        if constexpr (std::is_same_v<T, Char>) {
            return pred == c;
        } else if constexpr (std::is_convertible_v<T, Char const*>) {
            return std::strchr(pred, c) != nullptr;
        } else {
            return pred(c);
        }
    }
};

} // namespace ac::io
