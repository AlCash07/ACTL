// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <utility>

namespace ac {

template <class T>
struct out_t {
    T x;

    constexpr operator T() {
        return x;
    }

    template <class U>
    out_t& operator=(const U& y) {
        assign(*this, y);
        return *this;
    }
};

template <class T>
out_t<T> out(T&& x) {
    return {std::forward<T>(x)};
}

}  // namespace ac
