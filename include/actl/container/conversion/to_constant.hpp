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

template<auto To, class From>
struct conversion<constant<To>, From> {
    static constexpr bool value = std::is_constructible_v<decltype(To), From>;

    static constexpr constant<To> convert(From&& x
    ) noexcept(AC_ASSERT_IS_NOEXCEPT()) {
        AC_ASSERT(x == To);
        return {};
    }
};

template<auto To, auto From>
struct conversion<constant<To>, constant<From>> {
    static constexpr bool value = To == From;

    static constexpr constant<To> convert(constant<From>) noexcept {
        return {};
    }
};

} // namespace ac
