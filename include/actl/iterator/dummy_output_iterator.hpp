// Copyright 2019 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

namespace ac {

struct dummy_output_iterator
{
    using interator_category = std::output_iterator_tag;
    using value_type = void;
    using reference = void;
    using pointer = void;
    using difference_type = void;

    using R = dummy_output_iterator&;

    template <class T>
    R operator=(T const&)
    {
        return *this;
    }

    R operator*()
    {
        return *this;
    }

    R operator++()
    {
        return *this;
    }

    R operator++(int)
    {
        return *this;
    }
};

} // namespace ac
