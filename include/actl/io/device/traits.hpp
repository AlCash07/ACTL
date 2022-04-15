// Copyright 2022 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <type_traits>
#include <utility>

namespace ac::io {

struct device_base
{};

template <class Device>
using char_t = typename Device::char_type;

template <class T>
inline constexpr bool is_device_v = std::is_base_of_v<device_base, T>;

template <class T, class = void>
struct has_input_buffer : std::false_type
{};

template <class T>
struct has_input_buffer<
    T,
    std::void_t<decltype(std::declval<T>().input_data())>> : std::true_type
{};

template <class T, class = void>
struct has_output_buffer : std::false_type
{};

template <class T>
struct has_output_buffer<
    T,
    std::void_t<decltype(std::declval<T>().output_data())>> : std::true_type
{};

} // namespace ac::io
