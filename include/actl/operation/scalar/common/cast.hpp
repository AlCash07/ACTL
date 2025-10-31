// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/operation/scalar/scalar_operation.hpp>

namespace ac {

template<typename Target>
struct cast_f : scalar_operation<cast_f<Target>, 1> {
    using operation_category = scalar_operation_tag;

    template<typename T>
    static constexpr Target eval_scalar(T x) {
        return static_cast<Target>(x);
    }
};
template<typename T>
inline constexpr cast_f<T> cast;

} // namespace ac
