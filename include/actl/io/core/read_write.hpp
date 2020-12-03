/***************************************************************************************************
 * Copyright 2020 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/io/core/apply_format.hpp>
#include <actl/io/core/batch.hpp>
#include <actl/io/core/manipulator.hpp>
#include <actl/io/core/serialization_access.hpp>
#include <actl/range/traits/is_range.hpp>
#include <actl/util/span.hpp>

namespace ac::io {

/* Argument traits */

template <class T>
struct is_tuple : decltype(serialization_access{}.is_io_tuple<T>(0)) {};

template <class... Ts>
struct is_tuple<std::tuple<Ts...>> : std::true_type {};

template <class T>
inline constexpr bool is_tuple_v = is_tuple<T>::value;

}  // namespace ac::io

namespace ac::io::detail {

template <class B, index N, enable_int_if<std::is_arithmetic_v<B> && sizeof(B) == 1> = 0>
std::true_type is_byte_span(const span<B, N>&);

template <class T>
std::false_type is_byte_span(const T&);

template <class D, class F, class T>
index write_pre_final(D& od, F& fmt, const T& x) {
    if constexpr (!decltype(is_byte_span(x))::value && is_range_v<T> || is_tuple_v<T>) {
        manipulate(fmt, change_level{true});
        index res = write_final(od, fmt, x);
        manipulate(fmt, change_level{false});
        return res;
    } else {
        return write_final(od, fmt, x);
    }
}

template <class D, class F, class T>
bool read_pre_final(D& id, F& fmt, T&& x) {
    using U = remove_cvref_t<T>;
    if constexpr (!decltype(is_byte_span(x))::value && is_range_v<U> || is_tuple_v<U>) {
        manipulate(fmt, change_level{true});
        bool res = read_final(id, fmt, x);
        manipulate(fmt, change_level{false});
        return res;
    } else {
        return read_final(id, fmt, x);
    }
}

template <class D, class FF, class F, class T>
struct format_resolver {
    static index write(D& od, FF& full_fmt, F& fmt, const T& x) {
        return write_pre_final(od, full_fmt, apply_format_write(fmt, x));
    }

    static bool read(D& id, FF& full_fmt, F& fmt, T& x) {
        return read_pre_final(id, full_fmt, apply_format_read(fmt, x));
    }
};

template <class D, class FF, class F, class T, bool = is_manipulator<T>::value>
struct arg_resolver : format_resolver<D, FF, F, T> {};

template <class D, class FF, class F, class T>
struct arg_resolver<D, FF, F, T, true> {
    static index write(D&, FF&, F& fmt, const T& x) {
        manipulate(fmt, x);
        return 0;
    }

    static bool read(D&, FF&, F& fmt, T& x) {
        manipulate(fmt, x);
        return true;
    }
};

template <class D, class FF, class F, class T>
index write_impl(D& od, FF& full_fmt, F& fmt, const T& x) {
    return arg_resolver<D, FF, F, T>::write(od, full_fmt, fmt, x);
}

template <class D, class FF, class F, class T>
bool read_impl(D& id, FF& full_fmt, F& fmt, T&& x) {
    return arg_resolver<D, FF, F, std::remove_reference_t<T>>::read(id, full_fmt, fmt, x);
}

}  // namespace ac::io::detail
