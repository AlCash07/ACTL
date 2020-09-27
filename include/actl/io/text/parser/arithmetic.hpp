/***************************************************************************************************
 * Copyright 2020 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/io/core/parser_executor.hpp>
#include <actl/io/text/parser/integral.hpp>
#include <actl/io/text/text.hpp>

namespace ac::io {

template <class Format, class Int, enable_int_if_text<Format> = 0,
          enable_int_if<std::is_integral_v<Int> && !std::is_same_v<Int, char>> = 0>
inline auto deserialize(Format& fmt, Int& x) {
    return parser_executor<Int, integral_parser<Int>>{x, integral_parser<Int>{fmt.base}};
}

}  // namespace ac::io
