#pragma once

#include <cp/string/split.hpp>

namespace cp { namespace diagnostics { namespace detail {

inline std::vector<std::string> split_va_args(const std::string& va_args_str) {
    return split(va_args_str, [d = int(), q = bool(), lc = char()](char c) mutable {
        std::swap(lc, c);
        switch (lc) {
            case ',': {
                return d == 0 && !q;
            }
            case '"': {
                if (!q || c != '\\') {
                    q = !q;
                }
                return false;
            }
            case '(': {
                d += !q;
                return false;
            }
            case ')': {
                d -= !q;
                return false;
            }
        }
        return false;
    });
}

}}}  // namespace cp::diagnostics::detail