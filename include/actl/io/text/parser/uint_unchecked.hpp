/***************************************************************************************************
 * Copyright 2020 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/io/text/parser/digit.hpp>
#include <actl/util/span.hpp>

namespace ac::io {

template <class UInt>
class uint_unchecked_parser {
    UInt x = 0;

public:
    UInt base;

    template <digit_kind Kind>
    index parse(cspan<char> s) {
        index i = 0;
        for (; i != s.size(); ++i) {
            UInt d = to_digit<UInt, Kind>(s[i]);
            if (!is_digit(d, base))
                break;
            x = x * base + d;
        }
        return i;
    }

    bool ready() const { return true; }

    UInt value() const { return x; }
};

}  // namespace ac::io
