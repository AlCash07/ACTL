// Copyright 2022 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/io/concepts.hpp>

namespace ac::io {

template <Device Dev, class T>
    requires(std::is_empty_v<T> && !std::is_invocable_r_v<bool, T&, Dev&>)
bool write_final(Dev&, Format auto&, T&) {
    return true;
}

template <Device Dev, class T>
    requires(std::is_empty_v<T> && !std::is_invocable_r_v<bool, T&, Dev&>)
bool read_final(Dev&, Format auto&, T&) {
    return true;
}

} // namespace ac::io
