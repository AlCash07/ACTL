// Copyright 2022 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/meta/associated_types.hpp>
#include <iterator>

namespace ac {

template <std::input_or_output_iterator Iter>
struct associated_types<Iter>
{
    using value_type = std::iter_value_t<Iter>;
    using reference = std::iter_reference_t<Iter>;
};

} // namespace ac
