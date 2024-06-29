// Copyright 2019 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/io/argument/byte_span.hpp>
#include <actl/io/format/text/text.hpp>

namespace ac::io {

inline constexpr char true_s[4] = {'t', 'r', 'u', 'e'};
inline constexpr char false_s[5] = {'f', 'a', 'l', 's', 'e'};
inline constexpr char zero_c = '0';
inline constexpr char one_c = '1';

// We don't simply write bool, because that would allow implicit conversions.
cspan<char> encode(TextFormat auto& fmt, std::same_as<bool> auto x) {
    if (fmt.getf(flag::boolalpha))
        if (x)
            return true_s;
        else
            return false_s;
    else
        return {x ? &one_c : &zero_c, 1};
}

} // namespace ac::io
