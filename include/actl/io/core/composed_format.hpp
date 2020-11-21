/***************************************************************************************************
 * Copyright 2020 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/io/core/apply_format.hpp>
#include <actl/io/core/manipulator.hpp>

namespace ac::io {

template <class T, class = void>
struct is_format : std::false_type {};

template <class T>
struct is_format<T, std::void_t<typename T::format_tag>> : std::true_type {};

template <class T>
inline constexpr bool is_format_v = is_format<std::remove_reference_t<T>>::value;

template <class First, class Second>
struct composed {
    First first;
    Second second;

    struct format_tag;
};

template <class First, class Second, enable_int_if<is_format_v<First> && is_format_v<Second>> = 0>
auto operator >>= (First&& first, Second&& second) {
    return composed<First, Second>{std::forward<First>(first), std::forward<Second>(second)};
}

namespace detail {

template <class D, class FF, class First, class Second, class T>
struct format_resolver<D, FF, composed<First, Second>, T> {
    static index write(D& od, FF& full_fmt, composed<First, Second>& fmt, const T& x) {
        return write_impl(od, full_fmt, fmt.second, apply_format_write(fmt.first, x));
    }

    static bool read(D& id, FF& full_fmt, composed<First, Second>& fmt, T& x) {
        return read_impl(id, full_fmt, fmt.second, apply_format_read(fmt.first, x));
    }
};

}  // namespace detail

template <class First, class Second, class Manipulator>
void manipulate(composed<First, Second>& fmt, const Manipulator& m) {
    manipulate(fmt.first, m);
    manipulate(fmt.second, m);
}

}  // namespace ac::io
