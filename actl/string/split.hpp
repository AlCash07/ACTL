/***************************************************************************************************
 * Copyright 2018 Roman Rizvanov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <string>
#include <vector>

namespace ac {

template <class CharPredicate, class = decltype(std::declval<CharPredicate>()('\0'))>
inline std::vector<std::string> split(const std::string& line, CharPredicate is_delimiter,
                                      bool remove_empty = true) {
    std::vector<std::string> res(1);
    for (char c : line) {
        if (!is_delimiter(c)) {
            res.back() += c;
        } else if (!remove_empty || !res.back().empty()) {
            res.emplace_back();
        }
    }
    if ((remove_empty || res.size() == 1u) && res.back().empty()) res.pop_back();
    return res;
}

inline std::vector<std::string> split(const std::string& line, char delimiter,
                                      bool remove_empty = true) {
    return split(line, [delimiter](char c) { return c == delimiter; }, remove_empty);
}

}  // namespace ac
