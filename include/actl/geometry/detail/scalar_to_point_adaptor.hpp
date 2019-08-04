/***************************************************************************************************
 * Copyright 2017 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/geometry/line.hpp>
#include <iterator>

namespace ac::detail {

template <class Line, class It>
class scalar_to_point_adaptor {
public:
    using value_type = void;
    using difference_type = void;
    using pointer = void;
    using reference = void;
    using iterator_category = std::output_iterator_tag;

    explicit scalar_to_point_adaptor(const Line& line, It it) : line_{&line}, it_{it} {}

    scalar_to_point_adaptor& operator = (const scalar_to_point_adaptor&) = default;

    template <class T>
    void operator = (const T& x) {
        *it_ = (*line_)(x);
    }

    scalar_to_point_adaptor& operator*() { return *this; }
    scalar_to_point_adaptor& operator++() { return *this; }
    scalar_to_point_adaptor operator++(int) { return *this; }

    operator It() const { return it_; }

private:
    const Line* line_;
    It it_;
};

}  // namespace ac::detail
