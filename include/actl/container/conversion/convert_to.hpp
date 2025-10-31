// Copyright 2022 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/functional/noexcept/AC_DEDUCE_NOEXCEPT_AND_RETURN.hpp>
#include <type_traits>
#include <utility> // for std::forward

namespace ac {

template<typename Target, typename... Args>
struct conversion : std::false_type {};

template<typename Target>
    requires(std::is_default_constructible_v<Target>)
struct conversion<Target> : std::true_type {
    static constexpr Target convert() AC_DEDUCE_NOEXCEPT_AND_RETURN(Target{})
};

template<typename Target, typename Source>
    requires(std::is_arithmetic_v<Target> && std::is_convertible_v<Source, Target>)
struct conversion<Target, Source> : std::true_type {
    static constexpr Target convert(Source&& source)
        AC_DEDUCE_NOEXCEPT_AND_RETURN(
            static_cast<Target>(std::forward<Source>(source))
        )
};

template<typename Target, typename... Args>
inline constexpr bool can_convert_to_v = conversion<Target, Args...>::value;

template<typename Target, typename... Args>
constexpr Target convert_to(Args&&... args) AC_DEDUCE_NOEXCEPT_AND_RETURN(
    conversion<Target, Args...>::convert(std::forward<Args>(args)...)
)

} // namespace ac
