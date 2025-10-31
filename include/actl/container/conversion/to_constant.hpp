// Copyright 2022 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/AC_ASSERT.hpp>
#include <actl/container/conversion/convert_to.hpp>
#include <actl/meta/constant.hpp>

namespace ac {

template<auto Target, typename Source>
struct conversion<constant<Target>, Source> {
    static constexpr bool value =
        std::is_constructible_v<decltype(Target), Source>;

    static constexpr constant<Target> convert(Source&& source
    ) noexcept(AC_ASSERT_IS_NOEXCEPT()) {
        AC_ASSERT(source == Target);
        return {};
    }
};

template<auto Target, auto Source>
struct conversion<constant<Target>, constant<Source>> {
    static constexpr bool value = Target == Source;

    static constexpr constant<Target> convert(constant<Source>) noexcept {
        return {};
    }
};

} // namespace ac
