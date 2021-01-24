// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <type_traits>

namespace ac::io {

#define DEFINE_CAN(f)                                              \
    template <class... Ts>                                         \
    auto can_##f(Ts&... xs)->decltype(f(xs...), std::true_type{}); \
    std::false_type can_##f(...);

DEFINE_CAN(encode)
DEFINE_CAN(make_parser)

#undef DEFINE_CAN

template <class Format, class T>
decltype(auto) apply_format_write(Format& fmt, const T& x) {
    if constexpr (decltype(can_encode(fmt, x))::value)
        return encode(fmt, x);
    else
        return x;
}

template <class Format, class T>
decltype(auto) apply_format_read(Format& fmt, T& x) {
    if constexpr (decltype(can_make_parser(fmt, x))::value)
        return make_parser(fmt, x);
    else if constexpr (decltype(can_encode(fmt, x))::value)
        return encode(fmt, x);
    else
        return x;
}

} // namespace ac::io
