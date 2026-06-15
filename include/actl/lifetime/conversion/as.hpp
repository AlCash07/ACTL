// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/operation/operation/operation_base.hpp>

namespace ac {

template<typename Target>
struct As : operation_base<As<Target>> {
    template<typename T>
    static constexpr Target evaluate(T t) {
        return static_cast<Target>(t);
    }
};
template<typename T>
inline constexpr As<T> as;

} // namespace ac
