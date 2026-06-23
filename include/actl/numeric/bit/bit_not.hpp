// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/numeric/bit/bit_operation.hpp>

namespace ac {

namespace BitNot {
struct op : operation_base<op> {
    using operation_category = bitwise_operation;
};
} // namespace BitNot
inline constexpr BitNot::op bit_not;

template<typename T>
    requires EnableOperators<T>
constexpr auto operator~(T&& t) {
    return bit_not(std::forward<T>(t));
}

} // namespace ac
