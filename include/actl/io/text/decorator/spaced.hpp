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

/**
 * Format that inserts delimiter between consecutive output units.
 */
template <class Char = char>
class spaced {
public:
    struct format_tag;

    bool& separate() { return separate_; }

    cspan<Char> space() const { return space_; }
    void space(cspan<Char> x) { space_.assign(x.begin(), x.end()); }

private:
    std::basic_string<Char> space_ = " ";
    bool separate_ = false;
};

template <class C, class T, enable_int_if<!is_manipulator<T>::value> = 0>
inline tuple<cspan<C>, const T&> serialize(spaced<C>& fmt, const T& x) {
    if (fmt.separate()) {
        return {fmt.space(), x};
    } else {
        fmt.separate() = true;
        return {{}, x};
    }
}

template <class C, class T>
inline decltype(auto) serialize(spaced<C>& fmt, const raw<T>& x) {
    fmt.separate() = false;
    return x;
}

struct setspace {
    std::string_view value;

    struct is_manipulator;
};

template <class C>
inline void serialize(spaced<C>& fmt, setspace x) {
    fmt.space(x.value);
}

template <class C>
inline void change_depth(spaced<C>& fmt, bool deeper) {
    fmt.separate() = !deeper;
}

}  // namespace ac::io
