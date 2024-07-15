// Copyright 2024 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <cstddef>

namespace ac {

struct lifetime_counts {
    size_t default_constructor_count = 0;
    size_t destructor_count = 0;
    size_t move_constructor_count = 0;
    size_t move_assignment_count = 0;
    size_t copy_constructor_count = 0;
    size_t copy_assignment_count = 0;
};

/// Type that counts special functions invocations
/// across all the instances of the class, in particular:
/// - default constructor and destructor;
/// - move constructor and assignment operator;
/// - copy constructor and assignment operator.
/// @tparam Tag should be different in every use case of lifetime_counter
/// to avoid mixing the results and
/// to avoid concurrent modification of the same class variables.
template<class Tag>
struct lifetime_counter {
    static lifetime_counts counts;

    lifetime_counter() noexcept {
        ++counts.default_constructor_count;
    }
    ~lifetime_counter() noexcept {
        ++counts.destructor_count;
    }

    lifetime_counter(lifetime_counter&&) noexcept {
        ++counts.move_constructor_count;
    }
    lifetime_counter& operator=(lifetime_counter&&) noexcept {
        ++counts.move_assignment_count;
        return *this;
    }

    lifetime_counter(const lifetime_counter&) noexcept {
        ++counts.copy_constructor_count;
    }
    lifetime_counter& operator=(const lifetime_counter&) noexcept {
        ++counts.copy_assignment_count;
        return *this;
    }
};

template<class Tag>
lifetime_counts lifetime_counter<Tag>::counts = {};

} // namespace ac
