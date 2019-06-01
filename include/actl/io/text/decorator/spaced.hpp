/***************************************************************************************************
 * Copyright 2019 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/io/text/text.hpp>
#include <string>

namespace ac::io {

template <class Format>
struct spacer {};

struct setspace {
    czstring value;
};

/**
 * Format that inserts delimiter between consecutive output units.
 */
template <class Format>
class spaced : public Format {
public:
    using Format::Format;

    void reset() { first_ = true; }

    czstring space() const { return space_.data(); }
    void space(czstring value) { space_ = value; }

    template <class Device>
    friend index serialize(Device&, spaced& fmt, setspace x) {
        fmt.space(x.value);
        return 0;
    }

    template <class Device, class T>
    friend index serialize(Device& od, spaced& fmt, const T& x, spacer<Format>) {
        index space{};
        if (!fmt.first_) {
            space = od.write(span<const char>{fmt.space_});
            fmt.reset();
        }
        index res = serialize(od, fmt, x, format_tag_t<Format>{});
        if (res != 0) fmt.first_ = false;
        return space + res;
    }

private:
    std::string space_ = " ";
    bool first_ = true;
};

template <class Format>
struct format_traits<spaced<Format>> {
    using tag = spacer<Format>;
};

}  // namespace ac::io
