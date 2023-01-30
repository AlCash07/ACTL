// Copyright 2019 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/io/argument/range.hpp>
#include <actl/range/iterator_range.hpp>
#include <actl/range/traits/associative_range.hpp>

namespace ac::io {

struct braced
{
    struct format_tag;
};

template <Container AC>
    requires AssociativeRange<AC>
auto encode(braced, AC const& cont)
{
    return batch{'{', make_range(cont), '}'};
}

template <Container SC>
    requires SequenceRange<SC>
auto encode(braced, SC const& cont)
{
    return batch{'[', make_range(cont), ']'};
}

template <class T>
    requires(Tuple<T> || IO_Tuple<T>)
auto encode(braced, T const& x)
{
    return batch{'(', x, ')'};
}

} // namespace ac::io
