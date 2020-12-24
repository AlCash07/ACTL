// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/io/core/batch.hpp>
#include <actl/io/utility/flush.hpp>

namespace ac::io {

// Format that flushes after each unit.
struct unit_flush {
    struct format_tag;
};

template <class T>
auto encode(unit_flush&, const T& x) {
    return batch{x, flush_t{}};
}

}  // namespace ac::io
