// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/io/format/text/parser/digit.hpp>
#include <actl/range/span.hpp>

namespace ac::io {

template<class UInt>
class uint_simple_parser {
    enum class states { empty, ok, overflow };
    states state = states::empty;
    UInt x = 0;

public:
    UInt base;
    UInt max;

    template<digit_kind Kind>
    size_t parse_impl(cspan<char> s) {
        size_t i = 0;
        if (state == states::empty) {
            x = to_digit<UInt, Kind>(s[i]);
            if (is_digit_in_base(x, base)) {
                ++i;
                state = states::ok;
            } else {
                return i;
            }
        }
        UInt max_quotient = max / base;
        UInt max_remainder = max % base;
        for (; i != s.size(); ++i) {
            UInt d = to_digit<UInt, Kind>(s[i]);
            if (!is_digit_in_base(d, base))
                break;
            if (max_quotient < x || (max_quotient == x && max_remainder < d)) {
                x = max;
                state = states::overflow;
            } else {
                x = x * base + d;
            }
        }
        return i;
    }

    size_t parse(cspan<char> s) {
        return base <= 10 ? parse_impl<digit>(s) : parse_impl<alnum>(s);
    }

    bool empty() const {
        return state == states::empty;
    }

    bool ready() const {
        return state == states::ok;
    }

    UInt value() const {
        return x;
    }
};

} // namespace ac::io
