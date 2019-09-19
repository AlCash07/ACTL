/***************************************************************************************************
 * Copyright 2019 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/container/functions.hpp>
#include <actl/io/io.hpp>
#include <actl/range/traits.hpp>

namespace ac::io {

template <class R, class Device>
inline constexpr bool is_non_span_range_v =
    is_range_v<R> && !is_span<R, char_t<Device>>::value && !is_span<R, const char_t<Device>>::value;

template <class Device, class Format, class R, enable_int_if<is_non_span_range_v<R, Device>> = 0>
inline index serialize(Device& od, Format& fmt, const R& x) {
    index res{};
    if constexpr (is_container_v<R> && static_size_v<R> == dynamic_size) {
        res = write_size(od, fmt, x.size());
    }
    for (const auto& value : x) {
        res += write(od, fmt, value);
    }
    return res;
}

template <class Device, class Format, class R, enable_int_if<is_non_span_range_v<R, Device>> = 0>
inline bool deserialize(Device& id, Format& fmt, R& x) {
    if constexpr (is_container_v<R> && static_size_v<R> == dynamic_size) {
        decltype(x.size()) size{};
        if (!read_size(id, fmt, size)) return false;
        if constexpr (!is_random_access_container_v<R>) {
            for (; size > 0; --size) {
                value_t<R> value;
                if (!read(id, fmt, value)) return false;
                emplace(x, std::move(value));
            }
            return true;
        } else {
            x.resize(size);
        }
    }
    for (auto& value : x) {
        if (!read(id, fmt, value)) return false;
    }
    return true;
}

}  // namespace ac::io
