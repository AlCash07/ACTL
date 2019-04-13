/***************************************************************************************************
 * Copyright 2019 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/io/io.hpp>
#include <actl/traits/string_traits.hpp>
#include <string>

namespace ac::io {

/**
 * Format that inserts delimiter between consecutive non-string and non-char output units.
 */
template <class Format>
class spacer : public Format {
public:
    const char* space() const { return space_.data(); }
    void space(const char* value) { space_ = value; }

    bool insert() const { return insert_; }
    void insert(bool value) { insert_ = value; }

protected:
    std::string space_ = " ";
    bool insert_ = false;
};

struct setspace {
    const char* value;
    setspace(const char* v) : value{v} {}
};

template <class Device, class Format>
inline int write(Device&, spacer<Format>& fmt, setspace arg) {
    fmt.space(arg.value);
    return 0;
}

template <class Device, class Format, class T>
inline std::enable_if_t<!is_composite_v<T>, int> write(Device& od, spacer<Format>& fmt,
                                                       const T& arg) {
    int res = 0;
    if constexpr (is_string_v<T> || std::is_same_v<T, char>) {
        fmt.insert(false);
    } else {
        if (fmt.insert()) {
            res = write(od, static_cast<Format&>(fmt), fmt.space());
        } else {
            fmt.insert(true);
        }
    }
    return res + write(od, static_cast<Format&>(fmt), arg);
}

}  // namespace ac::io
