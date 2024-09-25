// Copyright 2017 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/range/iterator/interface/forward_iterator_interface.hpp>

namespace ac {

// https://en.cppreference.com/w/cpp/iterator/bidirectional_iterator
template<class Iter>
class bidirectional_iterator_interface
    : public forward_iterator_interface<Iter> {
public:
    using iterator_category = std::bidirectional_iterator_tag;

    // Post-decrement is a free function here so that it's not hidden by the
    // pre-decrement operator--() defined by the derived iterator.
    friend constexpr Iter operator--(Iter& iter, int) noexcept(
        noexcept(Iter{iter}, --iter)
    ) {
        Iter iter_copy = iter;
        --iter;
        return iter_copy;
    }

    // For clarification, see operator== in forward_iterator_interface.
    template<std::same_as<bidirectional_iterator_interface> bii>
    friend constexpr constant<true> operator==(bii, bii) noexcept {
        return {};
    }
};

} // namespace ac
