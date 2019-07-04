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

template <class Tag>
struct spaced_tag {
    using base = Tag;
};

/**
 * Format that inserts delimiter between consecutive output units.
 */
template <class Format, class Char = char, template <class> class Except = is_raw>
class spaced : public Format {
public:
    using format_tag = spaced_tag<typename Format::format_tag>;

    using Format::Format;

    void reset() { separate_ = false; }

    char_span<Char> space() const { return space_; }
    void space(char_span<Char> x) { space_.assign(x.begin(), x.end()); }

    template <class Device, class Spaced, class T, class Tag>
    friend std::enable_if_t<!is_manipulator<T>::value, index> serialize(Device& od, Spaced& fmt,
                                                                        const T& x,
                                                                        spaced_tag<Tag>) {
        index res{};
        if constexpr (Except<T>::value) {
            fmt.separate_ = false;
            res = serialize(od, fmt, x, Tag{});
            fmt.separate_ = false;
        } else {
            if (fmt.separate_) {
                res = od.write(fmt.space());
                fmt.reset();
            }
            res += serialize(od, fmt, x, Tag{});
            fmt.separate_ = true;
        }
        return res;
    }

private:
    std::basic_string<Char> space_ = " ";
    bool separate_ = false;
};

struct setspace {
    char_span<char> value;
};

template <>
struct is_manipulator<setspace> : std::true_type {};

template <class Device, class Format>
inline index serialize(Device&, Format& fmt, setspace x) {
    fmt.space(x.value);
    return 0;
}

}  // namespace ac::io
