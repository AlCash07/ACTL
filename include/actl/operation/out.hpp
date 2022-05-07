// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <utility>

namespace ac {

template <class T>
struct out
{
    T x;

    constexpr operator T()
    {
        return x;
    }

    template <class U>
    out& operator=(U const& y)
    {
        assign(*this, y);
        return *this;
    }
};

template <class T>
out(T&&) -> out<T>;

} // namespace ac
