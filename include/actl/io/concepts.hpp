// Copyright 2022 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt)

// All the concepts are in the same file because they are often used together.
#pragma once

#include <concepts>
#include <type_traits>

namespace ac::io {

template<typename T>
concept Format = requires { typename std::remove_reference_t<T>::format_tag; };

struct device_base {};

template<typename Device>
using char_t = typename Device::char_type;

template<typename T>
concept Device = std::derived_from<std::remove_reference_t<T>, device_base>;

template<typename T>
concept BufferedInputDevice = Device<T> && requires(T t) { t.input_buffer(); };

template<typename T>
concept BufferedOutputDevice =
    Device<T> && requires(T t) { t.output_buffer(); };

} // namespace ac::io
