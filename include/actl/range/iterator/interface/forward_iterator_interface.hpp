// Copyright 2017 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/range/iterator/interface/input_iterator_interface.hpp>

namespace ac {

// https://en.cppreference.com/w/cpp/iterator/forward_iterator
template<class Iter>
class forward_iterator_interface : public input_iterator_interface<Iter> {
public:
    using iterator_category = std::forward_iterator_tag;

    // This class adds no functionality to input_iterator_interface,
    // because Iter authors themselves need to provide operator==.
};

} // namespace ac
