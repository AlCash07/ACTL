// Copyright 2022 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/io/concepts.hpp>

namespace ac::io {

template<Device Dev, class T>
    requires std::is_invocable_r_v<bool, T&, Dev&>
bool write_final(Dev& id, Format auto&, T& f) {
    return f(id);
}

template<Device Dev, class T>
    requires std::is_invocable_r_v<bool, T&, Dev&>
bool read_final(Dev& od, Format auto&, T& f) {
    return f(od);
}

} // namespace ac::io
