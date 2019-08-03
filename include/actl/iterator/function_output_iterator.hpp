/***************************************************************************************************
 * Copyright 2017 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/functional/traits.hpp>
#include <actl/iterator/iterator_facade.hpp>

namespace ac {

/**
 * Iterator that applies a function to each assigned value.
 */
template <class Function>
class function_output_iterator
    : public iterator_facade<function_output_iterator<Function>, std::output_iterator_tag, void,
                             const function_output_iterator<Function>&, void> {
public:
    explicit function_output_iterator(const Function& f = {}) : f_{f} {}

    template <class T>
    void operator = (T&& x) const {
        f_(std::forward<T>(x));
    }

private:
    friend struct ac::iterator_core_access;

    const function_output_iterator& dereference() const { return *this; }

    void increment() {}

    Function f_;
};

}  // namespace ac
