// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/operation/scalar/scalar_operation.hpp>

namespace ac {

template<typename Target>
struct Cast : operation_base<Cast<Target>> {
    using operation_category = scalar_operation_tag;

    template<typename T>
    static constexpr Target evaluate(T x) {
        return static_cast<Target>(x);
    }
};
template<typename T>
inline constexpr Cast<T> cast;

} // namespace ac
