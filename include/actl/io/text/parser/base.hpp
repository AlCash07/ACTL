// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/range/span.hpp>
#include <cstdint>

namespace ac::io {

class base_parser {
    enum class states { empty, zero, done };

    states state = states::empty;
    uint8_t base;

    uint8_t set_base(uint8_t value) {
        if (base == 0)
            base = value;
        return base;
    }

    bool parse_zero(char c) {
        if (c == '0')
            return true;
        else {
            set_base(10);
            return false;
        }
    }

    bool parse_base(char c) {
        if (c == 'x' || c == 'X')
            return set_base(16) == 16;
        else if (c == 'b' || c == 'B')
            return set_base(2) == 2;
        else {
            set_base(8);
            return false;
        }
    }

public:
    explicit base_parser(uint8_t base) : base{base} {}

    index parse(cspan<char> s) {
        index i = 0;
        switch (state) {
            case states::empty:
                if (!parse_zero(s[i])) {
                    state = states::done;
                    break;
                }
                ++i;
                state = states::zero;
            case states::zero:
                if (i != s.size() && parse_base(s[i])) {
                    ++i;
                    state = states::done;
                }
            case states::done:;
        }
        return i;
    }

    bool zero() const {
        return state == states::zero;
    }
    bool ready() const {
        return state != states::empty;
    }

    uint8_t value() const {
        return base;
    }
};

} // namespace ac::io
