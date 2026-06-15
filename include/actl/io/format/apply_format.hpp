// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <type_traits>

namespace ac::io {

#define DEFINE_CAN(f)                                                \
    template<typename... Ts>                                         \
    auto can_##f(Ts&... ts) -> decltype(f(ts...), std::true_type{}); \
    std::false_type can_##f(...);

DEFINE_CAN(encode)
DEFINE_CAN(make_parser)

#undef DEFINE_CAN

template<typename T>
decltype(auto) apply_format_write(Format auto& fmt, T const& t) {
    if constexpr (decltype(can_encode(fmt, t))::value)
        return encode(fmt, t);
    else
        return t;
}

template<typename T>
decltype(auto) apply_format_read(Format auto& fmt, T& t) {
    if constexpr (decltype(can_make_parser(fmt, t))::value)
        return make_parser(fmt, t);
    else if constexpr (decltype(can_encode(fmt, t))::value)
        return encode(fmt, t);
    else
        return t;
}

} // namespace ac::io
