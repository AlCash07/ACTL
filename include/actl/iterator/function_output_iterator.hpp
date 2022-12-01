// Copyright 2017 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/functional/callable_traits.hpp>
#include <actl/iterator/facade/iterator_facade.hpp>

namespace ac {

/// Iterator that applies a function to each assigned value.
template <class Function>
class function_output_iterator
    : public iterator_facade<
          function_output_iterator<Function>,
          std::output_iterator_tag>
{
public:
    explicit function_output_iterator(Function const& f = {}) : f_{f} {}

    template <class T>
    void operator=(T&& x) const
    {
        f_(std::forward<T>(x));
    }

    function_output_iterator const& operator*() const noexcept
    {
        return *this;
    }

    function_output_iterator& operator++() noexcept
    {
        return *this;
    }

private:
    Function f_;
};

} // namespace ac
