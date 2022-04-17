// Copyright 2022 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/io/argument/byte.hpp>
#include <actl/io/core/const_data_parser.hpp>
#include <actl/io/core/parser_executor.hpp>
#include <actl/range/span.hpp>

namespace ac::io {

template <
    class Device,
    class Format,
    class B,
    size_t N,
    enable_int_if_byte<B> = 0>
bool read_final(Device& id, Format&, span<const B, N>& s)
{
    span sc{reinterpret_cast<const char*>(s.data()), s.size()};
    return parser_executor{const_data_parser{sc}}(id);
}

} // namespace ac::io
