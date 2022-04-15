// Copyright 2019 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/io/format/text/text.hpp>

namespace ac::io {

inline constexpr char true_s[4] = {'t', 'r', 'u', 'e'};
inline constexpr char false_s[5] = {'f', 'a', 'l', 's', 'e'};
inline constexpr char zero_c = '0';
inline constexpr char one_c = '1';

template <
    class Format,
    class Bool,
    enable_int_if_text<Format> = 0,
    enable_int_if<std::is_same_v<Bool, bool>> = 0>
cspan<char> encode(Format& fmt, Bool x)
{
    if (fmt.getf(flag::boolalpha))
        if (x)
            return true_s;
        else
            return false_s;
    else
        return {x ? &one_c : &zero_c, 1};
}

} // namespace ac::io
