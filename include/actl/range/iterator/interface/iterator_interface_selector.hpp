// Copyright 2017 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/range/iterator/interface/output_iterator_interface.hpp>
#include <actl/range/iterator/interface/random_access_iterator_interface.hpp>

namespace ac {

template<typename DerivedIterator, typename Category>
struct iterator_interface_selector;

/// When the iterator category isn't fixed
/// but instead deduced by some expression,
/// the following selector can be used to select
/// the correct iterator_interface template.
template<typename DerivedIterator, typename Category>
using iterator_interface_selector_t =
    typename iterator_interface_selector<DerivedIterator, Category>::type;

template<typename DerivedIterator>
struct iterator_interface_selector<DerivedIterator, std::output_iterator_tag> {
    using type = output_iterator_interface<DerivedIterator>;
};

template<typename DerivedIterator>
struct iterator_interface_selector<DerivedIterator, std::input_iterator_tag> {
    using type = input_iterator_interface<DerivedIterator>;
};

template<typename DerivedIterator>
struct iterator_interface_selector<DerivedIterator, std::forward_iterator_tag> {
    using type = forward_iterator_interface<DerivedIterator>;
};

template<typename DerivedIterator>
struct iterator_interface_selector<
    DerivedIterator,
    std::bidirectional_iterator_tag> {
    using type = bidirectional_iterator_interface<DerivedIterator>;
};

template<typename DerivedIterator>
struct iterator_interface_selector<
    DerivedIterator,
    std::random_access_iterator_tag> {
    using type = random_access_iterator_interface<DerivedIterator>;
};

} // namespace ac
