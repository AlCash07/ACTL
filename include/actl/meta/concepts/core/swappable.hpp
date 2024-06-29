// Copyright 2023 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <concepts>

namespace ac {

template<class T>
concept NothrowSwappable = std::swappable<T> && requires(T& a, T& b) {
    { std::ranges::swap(a, b) } noexcept;
};

template<class T, class U>
concept NothrowSwappableWith =
    std::swappable_with<T, U> && requires(T&& t, U&& u) {
        { std::ranges::swap(std::forward<T>(t), std::forward<T>(t)) } noexcept;
        { std::ranges::swap(std::forward<U>(u), std::forward<U>(u)) } noexcept;
        { std::ranges::swap(std::forward<T>(t), std::forward<U>(u)) } noexcept;
        { std::ranges::swap(std::forward<U>(u), std::forward<T>(t)) } noexcept;
    };

} // namespace ac
