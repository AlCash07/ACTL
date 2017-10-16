/***************************************************************************************************
 * Text I/O devices manipulation similar to http://en.cppreference.com/w/cpp/header/iomanip
 ***************************************************************************************************
 * Copyright 2017 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/io/device/text_input_device.hpp>
#include <actl/io/device/text_output_device.hpp>

namespace ac {

namespace detail {

struct base      { uint32_t    value; };
struct width     { uint32_t    value; };
struct fill      { char        value; };
struct precision { uint32_t    value; };
struct delimiter { const char* value; };

}  // namespace detail

static inline constexpr detail::base      set_base     (uint32_t    value = 10 ) { return {value}; }
static inline constexpr detail::width     set_width    (uint32_t    value = 0  ) { return {value}; }
static inline constexpr detail::fill      set_fill     (char        value = ' ') { return {value}; }
static inline constexpr detail::precision set_precision(uint32_t    value = 9  ) { return {value}; }
static inline constexpr detail::delimiter set_delimiter(const char* value = " ") { return {value}; }

static inline bool read(text_io_tag, text_input_device& in, detail::base base) {
    in.set_base(base.value);
    return true;
}

inline int write(text_io_tag, text_output_device& out, detail::width width) {
    out.set_width(width.value);
    return 0;
}

inline int write(text_io_tag, text_output_device& out, detail::fill fill) {
    out.set_fill(fill.value);
    return 0;
}

inline int write(text_io_tag, text_output_device& out, detail::base base) {
    out.set_base(base.value);
    return 0;
}

inline int write(text_io_tag, text_output_device& out, detail::precision precision) {
    out.set_precision(precision.value);
    return 0;
}

inline int write(text_io_tag, text_output_device& out, detail::delimiter delimiter) {
    out.set_delimiter(delimiter.value);
    return 0;
}

}  // namespace ac
