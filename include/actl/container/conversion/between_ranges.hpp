// Copyright 2022 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/container/conversion/convert_to.hpp>
#include <actl/range/is_dynamic_range.hpp>

namespace ac {

template <class To, class From>
struct conversion_sfinae<
    std::enable_if_t<is_dynamic_range_v<To> && is_dynamic_range_v<From>>,
    To,
    From>
{
    using from_iter = iterator_t<std::remove_reference_t<const From&>>;

    static constexpr bool value =
        std::is_constructible_v<To, from_iter, from_iter>;

    static constexpr To convert(const From& x)
        AC_DEDUCE_NOEXCEPT_AND_RETURN(To{std::begin(x), std::end(x)})
};

} // namespace ac