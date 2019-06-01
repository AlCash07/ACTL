/***************************************************************************************************
 * Copyright 2019 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/io/io.hpp>
#include <string>

namespace ac::io {

template <class Tag>
struct spaced_tag {
    using base = Tag;
};

struct setspace {
    ospan<const char> value;
};

template <>
struct is_manipulator<setspace> : std::true_type {};

/**
 * Format that inserts delimiter between consecutive output units.
 */
template <class Format, class Char = char, template <class> class Except = is_raw>
class spaced : public Format {
public:
    using Format::Format;

    void reset() { separate_ = false; }

    ospan<const Char> space() const { return space_; }
    void space(ospan<const Char> x) { space_.assign(x.begin(), x.end()); }

    template <class Device, class T, class Tag, class = std::enable_if_t<!is_manipulator<T>::value>>
    friend index serialize(Device& od, spaced& fmt, const T& x, spaced_tag<Tag>) {
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

    template <class Device>
    friend index serialize(Device&, spaced& fmt, setspace x) {
        fmt.space(x.value);
        return 0;
    }

private:
    std::basic_string<Char> space_ = " ";
    bool separate_ = false;
};

template <class Format, class Char>
struct format_traits<spaced<Format, Char>> {
    using tag = spaced_tag<format_tag_t<Format>>;
};

}  // namespace ac::io
