/***************************************************************************************************
 * Copyright 2017 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <type_traits>

namespace ac {

struct io_tag {};

struct binary_io_tag : io_tag {};
struct text_io_tag : io_tag {};

template <class Device>
struct io_traits {
    static constexpr bool binary = std::is_base_of_v<binary_io_tag, typename Device::category>;
    static constexpr bool text   = std::is_base_of_v<text_io_tag, typename Device::category>;

    static_assert(!binary || !text, "device can't be binary and text at the same time");
};

}  // namespace ac
