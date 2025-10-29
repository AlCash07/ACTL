// Copyright 2022 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/utility/none.hpp>

namespace ac {

/// Type for which the @p Range is a sub-range,
/// similarly to a superset for a set.
template<typename Range>
struct super_range {
    using type = none;
};

template<typename T>
using super_range_t = typename super_range<T>::type;

template<typename T>
inline constexpr bool has_super_range_v =
    !std::is_same_v<none, super_range_t<T>>;

} // namespace ac
