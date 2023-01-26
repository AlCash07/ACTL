// Copyright 2022 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/io/concepts.hpp>
#include <actl/meta/type_traits.hpp>

namespace ac::io {

template <
    Device Dev,
    class T,
    enable_int_if<
        std::is_empty_v<T> && !std::is_invocable_r_v<bool, T&, Dev&>> = 0>
bool write_final(Dev&, Format auto&, T&)
{
    return true;
}

template <
    Device Dev,
    class T,
    enable_int_if<
        std::is_empty_v<T> && !std::is_invocable_r_v<bool, T&, Dev&>> = 0>
bool read_final(Dev&, Format auto&, T&)
{
    return true;
}

} // namespace ac::io
