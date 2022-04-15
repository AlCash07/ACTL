// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/range/span.hpp>

namespace ac::io {

template <bool Signed>
class sign_parser
{
    bool negate = false;

public:
    size_t parse(cspan<char> s)
    {
        char c = s[0];
        if constexpr (Signed)
            if (c == '-')
                return negate = true;
        return c == '+';
    }

    template <class Int>
    auto max_abs() const
    {
        using UInt = std::make_unsigned_t<Int>;
        constexpr auto max_v = std::numeric_limits<UInt>::max();
        if constexpr (Signed)
            return max_v / 2 + UInt{negate};
        else
            return max_v;
    }

    template <class T>
    auto value(T x) const
    {
        if constexpr (std::is_floating_point_v<T>)
        {
            return negate ? -x : x;
        }
        else
        {
            using Int = std::make_signed_t<T>;
            if constexpr (Signed)
                return negate ? ~static_cast<Int>(x - 1) : static_cast<Int>(x);
            else
                return x;
        }
    }
};

} // namespace ac::io
