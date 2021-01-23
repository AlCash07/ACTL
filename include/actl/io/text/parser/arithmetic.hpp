// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/io/core/parser_executor.hpp>
#include <actl/io/text/parser/boolean.hpp>
#include <actl/io/text/parser/float_unchecked.hpp>
#include <actl/io/text/parser/integral.hpp>
#include <actl/io/text/text.hpp>

namespace ac::io {

template <class Format, enable_int_if_text<Format> = 0>
auto make_parser(Format& fmt, bool& x) {
    return parser_executor{x, boolean_parser{fmt.getf(flag::boolalpha)}};
}

template <class Format, class Int, enable_int_if_text<Format> = 0,
          enable_int_if<std::is_integral_v<Int> && !is_char_v<Int>> = 0>
auto make_parser(Format& fmt, Int& x) {
    return parser_executor{x, integral_parser<Int>{fmt.base}};
}

template <class Format, class Float, enable_int_if_text<Format> = 0,
          enable_int_if<std::is_floating_point_v<Float>> = 0>
auto make_parser(Format& fmt, Float& x) {
    return parser_executor{x, float_unchecked_parser<Float>{fmt.base}};
}

}  // namespace ac::io
