/***************************************************************************************************
 * Copyright 2019 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/io/text/text.hpp>

namespace ac::io {

constexpr char true_s[4] = {'t', 'r', 'u', 'e'};
constexpr char false_s[5] = {'f', 'a', 'l', 's', 'e'};
constexpr char zero_c = '0';
constexpr char one_c = '1';

template <class Format, class Bool, enable_int_if_text<Format> = 0,
          enable_int_if<std::is_same_v<Bool, bool>> = 0>
inline cspan<char> serialize(Format& fmt, Bool x) {
    if (fmt.getf(flags::boolalpha)) {
        if (x) {
            return true_s;
        } else {
            return false_s;
        }
    } else {
        if (x) {
            return {&one_c, 1};
        } else {
            return {&zero_c, 1};
        }
    }
}

}  // namespace ac::io
