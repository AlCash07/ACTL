// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/io/core/manipulator.hpp>
#include <actl/io/core/read_write.hpp>
#include <actl/io/format/apply_format.hpp>
#include <actl/io/format/traits.hpp>

namespace ac::io {

template <class First, class Second>
struct composed_format
{
    First first;
    Second second;

    struct format_tag;
};

template <
    class First,
    class Second,
    enable_int_if<is_format_v<First> && is_format_v<Second>> = 0>
auto operator>>=(First&& first, Second&& second)
{
    return composed_format<First, Second>{
        std::forward<First>(first), std::forward<Second>(second)};
}

namespace detail {

template <class D, class FF, class First, class Second, class T>
struct format_resolver<D, FF, composed_format<First, Second>, T>
{
    static bool write(
        D& od, FF& full_fmt, composed_format<First, Second>& fmt, T const& x)
    {
        return write_impl(
            od, full_fmt, fmt.second, apply_format_write(fmt.first, x));
    }

    static bool read(
        D& id, FF& full_fmt, composed_format<First, Second>& fmt, T& x)
    {
        return read_impl(
            id, full_fmt, fmt.second, apply_format_read(fmt.first, x));
    }
};

} // namespace detail

template <class First, class Second, class Manipulator>
void manipulate(composed_format<First, Second>& fmt, Manipulator const& m)
{
    manipulate(fmt.first, m);
    manipulate(fmt.second, m);
}

} // namespace ac::io
