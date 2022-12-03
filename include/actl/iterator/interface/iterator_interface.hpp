// Iterator interface is designed to produce complete iterator interface from a
// set of core functions. Adopted from boost (with simplifications):
// http://www.boost.org/doc/libs/1_65_1/libs/iterator/doc/iterator_interface.html
//
// Copyright 2017 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/iterator/interface/output_iterator_interface.hpp>
#include <actl/iterator/interface/random_access_iterator_interface.hpp>

namespace ac {

template <class Iter, class Category>
struct iterator_interface_selector
{
    using type = basic_iterator_interface<Iter>;
};

template <class Iter>
struct iterator_interface_selector<Iter, std::input_iterator_tag>
{
    using type = input_iterator_interface<Iter>;
};

template <class Iter>
struct iterator_interface_selector<Iter, std::forward_iterator_tag>
{
    using type = forward_iterator_interface<Iter>;
};

template <class Iter>
struct iterator_interface_selector<Iter, std::bidirectional_iterator_tag>
{
    using type = bidirectional_iterator_interface<Iter>;
};

template <class Iter>
struct iterator_interface_selector<Iter, std::random_access_iterator_tag>
{
    using type = random_access_iterator_interface<Iter>;
};

template <class Iter>
struct iterator_interface_selector<Iter, std::output_iterator_tag>
{
    using type = output_iterator_interface<Iter>;
};

template <class Iter, class Category>
using iterator_interface_selector_t =
    typename iterator_interface_selector<Iter, Category>::type;

} // namespace ac
