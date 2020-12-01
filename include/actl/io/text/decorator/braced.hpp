/***************************************************************************************************
 * Copyright 2019 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/io/range.hpp>
#include <actl/io/util/raw.hpp>
#include <actl/range/iterator_range.hpp>
#include <actl/range/traits/range_traits.hpp>

namespace ac::io {

struct braced {
    struct format_tag;

    std::basic_string<char> key_value_separator = ":";
};

namespace detail {

template <class It>
struct map_range : iterator_range<It> {
    cspan<char> key_value_separator;
};

template <class Device, class Format, class It>
inline index write_final(Device& od, Format& fmt, map_range<It> r) {
    index res{};
    for (const auto& [key, value] : r) {
        res += write(od, fmt, key, raw{r.key_value_separator}, value);
    }
    return res;
}

template <class AC>
inline auto make_map_range(braced& fmt, const AC& cont) {
    if constexpr (is_simple_associative_range_v<AC>) {
        return make_range(cont);
    } else {
        return map_range<iterator_t<const AC>>{{cont.begin(), cont.end()},
                                               span{fmt.key_value_separator}};
    }
}

}  // namespace detail

template <class AC, enable_int_if<is_container_v<AC> && is_associative_range_v<AC>> = 0>
auto encode(braced& fmt, const AC& cont) {
    return batch{'{', detail::make_map_range(fmt, cont), '}'};
}

template <class SC, enable_int_if<is_container_v<SC> && is_sequence_range_v<SC>> = 0>
auto encode(braced, const SC& cont) {
    return batch{'[', make_range(cont), ']'};
}

template <class T, enable_int_if<is_tuple<T>::value> = 0>
auto encode(braced, const T& x) {
    return batch{'(', x, ')'};
}

}  // namespace ac::io
