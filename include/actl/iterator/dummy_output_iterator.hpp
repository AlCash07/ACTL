// Copyright 2019 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/iterator/facade/iterator_types.hpp>

namespace ac {

struct dummy_output_iterator : output_iterator_types {
    using R = dummy_output_iterator&;

    template <class T>
    R operator=(const T&) {
        return *this;
    }

    R operator*() {
        return *this;
    }
    R operator++() {
        return *this;
    }
    R operator++(int) {
        return *this;
    }
};

}  // namespace ac
