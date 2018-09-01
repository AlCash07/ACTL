#pragma once

namespace cp {

template <class LeftF, class RightF>
inline std::string trim(const std::string& str, LeftF left_skip_pred, RightF right_skip_pred) {
    auto i = str.begin();
    auto j = str.end();
    for (; i != j && left_skip_pred(i[0]); ++i);
    for (; i != j && right_skip_pred(j[-1]); --j);
    return std::string(i, j);
}

template <class F>
inline std::string trim(const std::string& str, F skipPredicate) { return trim(str, skipPredicate, skipPredicate); }

inline std::string trim(const std::string& str) { return trim(str, isspace); }

}  // namespace cp