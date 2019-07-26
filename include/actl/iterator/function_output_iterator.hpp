/***************************************************************************************************
 * Copyright 2017 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/functional/traits.hpp>
#include <actl/iterator/iterator_facade.hpp>
#include <actl/util/type_traits.hpp>

namespace ac {

/**
 * Iterator that applies a function to each assigned value.
 */
template <class UnaryFunction, class T = argument_t<UnaryFunction, 0>>
class function_output_iterator
    : public iterator_facade<function_output_iterator<UnaryFunction, T>,
                             std::output_iterator_tag,
                             remove_cvref_t<T>,
                             const function_output_iterator<UnaryFunction, T>&,
                             remove_cvref_t<T>*,
                             void> {
public:
    explicit function_output_iterator(const UnaryFunction& f = {}) : f_{f} {}

    void operator = (T arg) const { f_(arg); }

private:
    friend struct ac::iterator_core_access;

    const function_output_iterator& dereference() const { return *this; }

    void increment() {}

    UnaryFunction f_;
};

}  // namespace ac
