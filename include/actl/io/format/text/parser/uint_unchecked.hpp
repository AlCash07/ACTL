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
class uint_unchecked_parser {
    UInt x = 0;
    UInt base;

public:
    explicit uint_unchecked_parser(UInt base) : base{base} {}

    template<digit_kind Kind>
    size_t parse_impl(cspan<char> s) {
        size_t i = 0;
        for (; i != s.size(); ++i) {
            UInt d = to_digit<UInt, Kind>(s[i]);
            if (!is_digit_in_base(d, base))
                break;
            x = x * base + d;
        }
        return i;
    }

    size_t parse(cspan<char> s) {
        return base <= 10 ? parse_impl<digit>(s) : parse_impl<alnum>(s);
    }

    bool ready() const {
        return true;
    }

    UInt value() const {
        return x;
    }
};

} // namespace ac::io
