// Copyright 2018 Roman Rizvanov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/std/string.hpp>
#include <actl/string/char_classes.hpp>

namespace ac {

template<class CharPredicate0, class CharPredicate1>
std::string trim(
    std::string const& str, CharPredicate0 skip_left, CharPredicate1 skip_right
) {
    auto l = str.begin();
    auto r = str.end();
    for (; l != r && skip_left(l[0]); ++l) {}
    for (; l != r && skip_right(r[-1]); --r) {}
    return std::string(l, r);
}

template<class CharPredicate>
std::string trim(std::string const& str, CharPredicate skip) {
    return trim(str, skip, skip);
}

inline std::string trim(std::string const& str) {
    return trim(str, is_space);
}

} // namespace ac
