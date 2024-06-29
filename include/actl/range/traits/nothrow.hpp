// Copyright 2022 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/range/traits/associated_types.hpp>
#include <utility> // for std::declval

namespace ac {

template <class Range>
inline constexpr bool is_nothrow_iterable_v =
    noexcept(noexcept(
        ranges::begin(std::declval<Range>()) !=
        ranges::end(std::declval<Range>())
    ) //
                 && noexcept(++std::declval<range_iterator_t<Range>&>()));

} // namespace ac
