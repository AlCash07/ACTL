/***************************************************************************************************
 * Copyright 2018 Roman Rizvanov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/std/string.hpp>
#include <actl/string/ctype.hpp>

namespace ac {

template <class CharPredicate0, class CharPredicate1>
inline std::string trim(const std::string& str, CharPredicate0 skip_left,
                        CharPredicate1 skip_right) {
    auto l = str.begin();
    auto r = str.end();
    for (; l != r && skip_left(l[0]); ++l);
    for (; l != r && skip_right(r[-1]); --r);
    return std::string(l, r);
}

template <class CharPredicate>
inline std::string trim(const std::string& str, CharPredicate skip) {
    return trim(str, skip, skip);
}

inline std::string trim(const std::string& str) { return trim(str, is_space); }

}  // namespace ac
