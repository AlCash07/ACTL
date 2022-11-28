// Currently, all standard output iterators have associated output type, but
// don't provide typedef for it. output_type trait serves this purpose.
//
// Copyright 2017 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/range/traits/dependent.hpp>
#include <iterator>

namespace ac {

template <class OutIter>
struct output_type
{
    using type = std::iter_value_t<OutIter>;
};

template <class T, class CharT, class Traits>
struct output_type<std::ostream_iterator<T, CharT, Traits>>
{
    using type = T;
};

template <class Container>
struct output_type<std::back_insert_iterator<Container>>
{
    using type = range_value_t<Container>;
};

template <class Container>
struct output_type<std::front_insert_iterator<Container>>
{
    using type = range_value_t<Container>;
};

template <class Container>
struct output_type<std::insert_iterator<Container>>
{
    using type = range_value_t<Container>;
};

template <class OutIter>
using output_type_t = typename output_type<OutIter>::type;

} // namespace ac
