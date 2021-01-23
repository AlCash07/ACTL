// Copyright 2017 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/iterator/facade/iterator_facade.hpp>
#include <actl/meta/function_traits.hpp>

namespace ac {

/// Iterator that applies a function to each assigned value.
template <class Function>
class function_output_iterator
    : public iterator_facade<function_output_iterator<Function>, output_iterator_types> {
public:
    explicit function_output_iterator(const Function& f = {}) : f_{f} {}

    template <class T>
    void operator=(T&& x) const {
        f_(std::forward<T>(x));
    }

private:
    friend struct ac::iterator_core_access;

    const function_output_iterator& dereference() const {
        return *this;
    }

    void increment() {}

    Function f_;
};

}  // namespace ac
