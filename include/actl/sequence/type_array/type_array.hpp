// Copyright 2025 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <cstddef>

namespace ac {

/// Array of types intended for meta-programming.
/// Should be preferred over `std::tuple` when
/// the types don't have to be instantiated
/// because of improved compilation time, see
/// https://marzer.github.io/md_blog_2021_05_31_compilation_speed_humps_std_tuple.html
template<typename... Types>
struct type_array {
    static constexpr size_t length = sizeof...(Types);
};

} // namespace ac
