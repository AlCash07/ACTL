// Copyright 2022 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/container/array/semi_static_array.hpp>

namespace ac {

namespace detail {

template <size_t Size, size_t SizeDynamic, class T, T... Values>
struct ssa_selector
{
    using type = ac::semi_static_array<T, Values...>;
};

template <size_t N, class T, T... Values>
struct ssa_selector<N, 0, T, Values...>
{
    using type = ac::static_array<T, Values...>;
};

template <size_t N, class T, T... Values>
struct ssa_selector<N, N, T, Values...>
{
    using type = std::array<T, sizeof...(Values)>;
};

// This specialization is needed to disambiguate between the two above when size
// is 0.
template <class T>
struct ssa_selector<0, 0, T>
{
    using type = ac::static_array<T>;
};

} // namespace detail

template <class T, T... Values>
struct semi_static_array_selector
    : detail::ssa_selector<
          sizeof...(Values),
          (0 + ... + size_t{Values == dynamic_extent<T>}),
          T,
          Values...>
{};

template <class T, T... Values>
using semi_static_array_t =
    typename semi_static_array_selector<T, Values...>::type;

} // namespace ac
