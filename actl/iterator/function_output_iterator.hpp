/***************************************************************************************************
 * Copyright 2017 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/iterator/iterator_facade.hpp>

namespace ac {

/**
 * Iterator that applies a function to each assigned value.
 */
template <class UnaryFunction>
class function_output_iterator
    : public iterator_facade<function_output_iterator<UnaryFunction>,
                             std::output_iterator_tag,
                             void,
                             const function_output_iterator<UnaryFunction>&,
                             void,
                             void> {
public:
    explicit function_output_iterator(const UnaryFunction& f = UnaryFunction{}) : f_{f} {}

    template <class T>
    const function_output_iterator& operator = (const T& value) const {
        f_(value);
        return *this;
    }

private:
    friend struct iterator_core_access;

    const function_output_iterator& dereference() const { return *this; }

    void increment() {}

    UnaryFunction f_;
};

template <class UnaryFunction>
inline auto make_function_output_iterator(const UnaryFunction& f) {
    return function_output_iterator<UnaryFunction>(f);
}

}  // namespace ac
