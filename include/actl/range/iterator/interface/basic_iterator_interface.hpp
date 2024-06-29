// Copyright 2017 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <iterator>

namespace ac {

// https://en.cppreference.com/w/cpp/iterator/input_or_output_iterator
template <class Iter>
class basic_iterator_interface {
public:
    // Post-increment is a free function here so that it's not hidden by the
    // pre-increment operator++() defined by the derived iterator.
    // TODO: std::input_or_output_iterator doesn't require the return type here
    // to be exactly Iter
    // https://en.cppreference.com/w/cpp/iterator/weakly_incrementable
    friend constexpr Iter operator++(Iter& iter, int) noexcept(
        noexcept(Iter{iter}, ++iter)
    ) {
        Iter iter_copy = iter;
        ++iter;
        return iter_copy;
    }
};

} // namespace ac
