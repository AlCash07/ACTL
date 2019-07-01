/***************************************************************************************************
 * Currently, all standard output iterators have associated output type, but don't provide typedef
 * for it. output_type trait serves this purpose.
 ***************************************************************************************************
 * Copyright 2017 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <iterator>

namespace ac {

template <class OutputIterator>
struct output_type {
    using type = typename std::iterator_traits<OutputIterator>::value_type;
};

template <class T, class CharT, class Traits>
struct output_type<std::ostream_iterator<T, CharT, Traits>> {
    using type = T;
};

template <class Container>
struct output_type<std::back_insert_iterator<Container>> {
    using type = typename Container::value_type;
};

template <class Container>
struct output_type<std::front_insert_iterator<Container>> {
    using type = typename Container::value_type;
};

template <class Container>
struct output_type<std::insert_iterator<Container>> {
    using type = typename Container::value_type;
};

template <class OutputIterator>
using output_type_t = typename output_type<OutputIterator>::type;

}  // namespace ac
