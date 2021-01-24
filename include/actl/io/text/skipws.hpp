// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/io/core/batch.hpp>
#include <actl/io/utility/skip.hpp>
#include <actl/string/traits.hpp>

namespace ac::io {

struct skipws {
    struct format_tag;
};

template <class T, enable_int_if<std::is_arithmetic_v<T> || is_string_v<T>> = 0>
auto encode(skipws&, T& x) {
    return batch{ws, x};
}

} // namespace ac::io
