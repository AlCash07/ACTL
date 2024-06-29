// Copyright 2019 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/io/argument/byte_span.hpp>
#include <actl/io/device/mode.hpp>
#include <actl/meta/type_traits.hpp>
#include <actl/range/span.hpp>

namespace ac::io {

struct binary {
    struct format_tag;
};

template <class T>
concept BinaryDevice = Device<T> && is_bin<T::mode>;

binary deduce_format(BinaryDevice auto&) {
    return {};
}

template <class T>
    requires std::is_arithmetic_v<T>
auto encode(binary, T& x) {
    using byte_t = add_const_if_t<std::is_const_v<T>, std::byte>;
    return span<byte_t, sizeof(T)>{reinterpret_cast<byte_t*>(&x), sizeof(T)};
}

} // namespace ac::io
