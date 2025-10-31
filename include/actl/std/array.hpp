// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/meta/rebind.hpp>
#include <actl/range/traits/properties.hpp>
#include <array>

namespace ac {

template<typename T, size_t N>
struct range_properties<std::array<T, N>> : default_range_properties {
    static constexpr bool is_container = true;
};

template<typename T, size_t N>
struct template_type<std::array<T, N>> {
    using type = T;
};

template<typename T, size_t N, typename Target>
struct rebind<std::array<T, N>, Target> {
    using type = std::array<Target, N>;
};

} // namespace ac
