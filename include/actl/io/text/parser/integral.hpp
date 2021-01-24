// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/io/text/parser/base.hpp>
#include <actl/io/text/parser/sign.hpp>
#include <actl/io/text/parser/uint_simple.hpp>
#include <actl/io/text/text.hpp>

namespace ac::io {

template <class Int>
class integral_parser {
    enum class states { empty, sign, value };

    states state = states::empty;
    sign_parser<std::is_signed_v<Int>> sign;
    base_parser base;
    uint_simple_parser<std::make_unsigned_t<Int>> inner;

public:
    explicit integral_parser(uint8_t base) : base{base} {}

    index parse(cspan<char> s) {
        index i = 0;
        switch (state) {
            case states::empty:
                i += sign.parse(s);
                state = states::sign;
                if (i == s.size())
                    break;
            case states::sign:
                i += base.parse(s.subspan(i));
                if (i == s.size() || !base.ready())
                    break;
                inner.base = base.value();
                inner.max = sign.template max_abs<Int>();
                state = states::value;
            case states::value:
                i += inner.parse(s.subspan(i));
        }
        return i;
    }

    bool ready() const {
        return inner.ready() || (inner.empty() && base.zero());
    }

    Int value() const {
        return sign.value(inner.value());
    }
};

} // namespace ac::io
