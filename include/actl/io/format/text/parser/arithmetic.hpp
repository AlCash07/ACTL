// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/io/core/parser_executor.hpp>
#include <actl/io/format/text/parser/boolean.hpp>
#include <actl/io/format/text/parser/float_unchecked.hpp>
#include <actl/io/format/text/parser/integral.hpp>
#include <actl/io/format/text/text.hpp>

namespace ac::io {

auto make_parser(TextFormat auto& fmt, bool& x)
{
    return parser_executor{x, boolean_parser{fmt.getf(flag::boolalpha)}};
}

template <
    class Int,
    enable_int_if<std::is_integral_v<Int> && !is_char_v<Int>> = 0>
auto make_parser(TextFormat auto& fmt, Int& x)
{
    return parser_executor{x, integral_parser<Int>{fmt.base}};
}

template <class Float, enable_int_if<std::is_floating_point_v<Float>> = 0>
auto make_parser(TextFormat auto& fmt, Float& x)
{
    return parser_executor{x, float_unchecked_parser<Float>{fmt.base}};
}

} // namespace ac::io
