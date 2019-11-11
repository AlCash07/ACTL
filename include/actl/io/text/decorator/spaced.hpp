/***************************************************************************************************
 * Copyright 2019 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/io/text/text.hpp>
#include <actl/std/string.hpp>

namespace ac::io {

struct setspace {
    std::string_view value;

    struct is_manipulator;
};

/**
 * Format that inserts delimiter between consecutive output units.
 */
template <class Char = char, template <class> class Except = is_raw>
class spaced {
public:
    struct format_tag;

    void reset() { separate_ = false; }

    cspan<Char> space() const { return space_; }
    void space(cspan<Char> x) { space_.assign(x.begin(), x.end()); }

    template <class T, enable_int_if<!is_manipulator<T>::value> = 0>
    friend decltype(auto) serialize(spaced& fmt, const T& x) {
        if constexpr (Except<T>::value) {
            fmt.separate_ = false;
            return x;
        } else {
            tuple<cspan<Char>, const T&> res{{}, x};
            if (fmt.separate_) {
                std::get<0>(res) = fmt.space();
            } else {
                fmt.separate_ = true;
            }
            return res;
        }
    }

    friend void serialize(spaced& fmt, setspace x) { fmt.space(x.value); }

private:
    std::basic_string<Char> space_ = " ";
    bool separate_ = false;
};

}  // namespace ac::io
