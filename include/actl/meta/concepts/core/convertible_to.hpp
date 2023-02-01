// Copyright 2023 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <concepts>

namespace ac {

template <class From, class To>
concept NothrowExplicitlyConvertibleTo =
    requires {
        {
            static_cast<To>(std::declval<From>())
        } noexcept;
    };

template <class From, class To>
concept NothrowConvertibleTo =
    std::convertible_to<From, To> && std::is_nothrow_convertible_v<From, To> &&
    NothrowExplicitlyConvertibleTo<From, To>;

template <class From, class To>
concept MayThrowConvertibleTo = std::convertible_to<From, To> && !
std::is_nothrow_convertible_v<From, To> &&
    !NothrowExplicitlyConvertibleTo<From, To>;

} // namespace ac
