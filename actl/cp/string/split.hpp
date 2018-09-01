#pragma once

#include <vector>
#include <string>

namespace cp {

template <class F, class = decltype(std::declval<F>()('\0'))>
inline std::vector<std::string> split(const std::string& line, F delim_predicate, bool remove_empty = true) {
    std::vector<std::string> res(1);
    for (char c : line) {
        if (!delim_predicate(c)) {
            res.back() += c;
        } else if (!remove_empty || !res.back().empty()) {
            res.emplace_back();
        }
    }
    if ((remove_empty || (int)res.size() == 1) && res.back().empty())
        res.pop_back();
    return res;
}

inline std::vector<std::string> split(const std::string& line, char delim, bool remove_empty = true) {
    return split(line, [delim](char c) { return c == delim; }, remove_empty);
}

}  // namespace cp
