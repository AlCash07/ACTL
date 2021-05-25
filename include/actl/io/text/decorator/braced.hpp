// Copyright 2019 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/io/range.hpp>
#include <actl/range/iterator_range.hpp>
#include <actl/range/traits.hpp>

namespace ac::io {

struct braced
{
    struct format_tag;
};

template <
    class AC,
    enable_int_if<is_container_v<AC> && is_associative_range_v<AC>> = 0>
auto encode(braced, const AC& cont)
{
    return batch{'{', make_range(cont), '}'};
}

template <
    class SC,
    enable_int_if<is_container_v<SC> && is_sequence_range_v<SC>> = 0>
auto encode(braced, const SC& cont)
{
    return batch{'[', make_range(cont), ']'};
}

template <class T, enable_int_if<is_io_tuple_v<T>> = 0>
auto encode(braced, const T& x)
{
    return batch{'(', x, ')'};
}

} // namespace ac::io
