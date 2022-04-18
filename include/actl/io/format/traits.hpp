// Copyright 2022 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <type_traits>

namespace ac::io {

template <class T, class = void>
struct is_format : std::false_type
{};

template <class T>
struct is_format<T, std::void_t<typename T::format_tag>> : std::true_type
{};

template <class T>
inline constexpr bool is_format_v =
    is_format<std::remove_reference_t<T>>::value;

template <class T, class Tag, class = void>
struct has_format_tag : std::false_type
{};

template <class T, class Tag>
struct has_format_tag<T, Tag, std::void_t<typename T::format_tag>>
    : std::is_same<typename T::format_tag, Tag>
{};

} // namespace ac::io
