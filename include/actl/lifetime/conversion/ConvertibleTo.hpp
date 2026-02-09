// Copyright 2023 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <concepts>

namespace ac {

template<typename Source, typename Target>
concept NothrowExplicitlyConvertibleTo = requires {
    { static_cast<Target>(std::declval<Source>()) } noexcept;
};

template<typename Source, typename Target>
concept NothrowConvertibleTo = std::convertible_to<Source, Target> &&
                               std::is_nothrow_convertible_v<Source, Target> &&
                               NothrowExplicitlyConvertibleTo<Source, Target>;

template<typename Source, typename Target>
concept MayThrowConvertibleTo =
    std::convertible_to<Source, Target> &&
    !std::is_nothrow_convertible_v<Source, Target> &&
    !NothrowExplicitlyConvertibleTo<Source, Target>;

} // namespace ac
