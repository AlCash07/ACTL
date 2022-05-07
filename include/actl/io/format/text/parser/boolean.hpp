// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/io/core/const_data_parser.hpp>
#include <actl/io/format/text/arithmetic/boolean.hpp>

namespace ac::io {

struct boolean_parser
{
    enum class states
    {
        empty,
        suffix
    };
    bool x;
    bool const alpha;
    states state = states::empty;
    const_data_parser suffix_parser;

public:
    explicit boolean_parser(bool alpha) : alpha{alpha} {}

    size_t parse(cspan<char> s)
    {
        if (state == states::empty)
        {
            if (alpha)
            {
                x = s[0] == true_s[0];
                suffix_parser =
                    x ? const_data_parser{true_s} : const_data_parser{false_s};
                state = states::suffix;
            }
            else
            {
                x = s[0] == one_c;
                bool ok = x || s[0] == zero_c;
                if (ok)
                    state = states::suffix;
                return ok;
            }
        }
        return suffix_parser.parse(s);
    }

    bool ready() const
    {
        return suffix_parser.ready() && state != states::empty;
    }

    bool value() const
    {
        return x;
    }
};

} // namespace ac::io
