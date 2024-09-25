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

    // operator== is defined here to support defining it as = default
    // in the derived Iter.
    //
    // The parameters are constrained here to make sure this operator
    // doesn't get used by mistake for the instances of the derived Iter
    // after an implicit conversion to the base class.
    template<std::same_as<forward_iterator_interface> fii>
    friend constexpr constant<true> operator==(fii, fii) noexcept {
        return {};
    }
};

} // namespace ac
