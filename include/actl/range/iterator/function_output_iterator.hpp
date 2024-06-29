// Copyright 2017 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/functional/callable_traits.hpp>
#include <actl/range/iterator/interface/output_iterator_interface.hpp>

namespace ac {

/// Iterator that applies a function to each assigned value.
template<class Function>
class function_output_iterator
    : public output_iterator_interface<function_output_iterator<Function>> {
public:
    explicit function_output_iterator(Function const& f = {}) : f_{f} {}

    template<class T>
    void operator=(T&& x) const {
        f_(std::forward<T>(x));
    }

    function_output_iterator const& operator*() const noexcept {
        return *this;
    }

    function_output_iterator& operator++() noexcept {
        return *this;
    }

private:
    Function f_;
};

} // namespace ac
