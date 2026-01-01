// Copyright 2023 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <concepts>

namespace ac {

template<typename T>
concept NothrowSwappable = std::swappable<T> && requires(T& l, T& r) {
    { std::ranges::swap(l, r) } noexcept;
};

template<typename L, typename R>
concept NothrowSwappableWith =
    std::swappable_with<L, R> && requires(L&& l, R&& r) {
        { std::ranges::swap(std::forward<L>(l), std::forward<L>(l)) } noexcept;
        { std::ranges::swap(std::forward<R>(r), std::forward<R>(r)) } noexcept;
        { std::ranges::swap(std::forward<L>(l), std::forward<R>(r)) } noexcept;
        { std::ranges::swap(std::forward<R>(r), std::forward<L>(l)) } noexcept;
    };

} // namespace ac
