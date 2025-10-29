// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/io/concepts.hpp>
#include <actl/io/core/manipulator.hpp>
#include <actl/io/core/read_write.hpp>
#include <actl/io/format/apply_format.hpp>

namespace ac::io {

template<typename First, typename Second>
struct composed_format {
    First first;
    Second second;

    struct format_tag;
};

template<Format First, Format Second>
auto operator>>=(First&& first, Second&& second) {
    return composed_format<First, Second>{
        std::forward<First>(first), std::forward<Second>(second)
    };
}

namespace detail {

template<typename D, typename FF, typename First, typename Second, typename T>
struct format_resolver<D, FF, composed_format<First, Second>, T> {
    static bool write(
        D& od, FF& full_fmt, composed_format<First, Second>& fmt, T const& x
    ) {
        return write_impl(
            od, full_fmt, fmt.second, apply_format_write(fmt.first, x)
        );
    }

    static bool read(
        D& id, FF& full_fmt, composed_format<First, Second>& fmt, T& x
    ) {
        return read_impl(
            id, full_fmt, fmt.second, apply_format_read(fmt.first, x)
        );
    }
};

} // namespace detail

template<typename First, typename Second, typename Manipulator>
void manipulate(composed_format<First, Second>& fmt, Manipulator const& m) {
    manipulate(fmt.first, m);
    manipulate(fmt.second, m);
}

} // namespace ac::io
