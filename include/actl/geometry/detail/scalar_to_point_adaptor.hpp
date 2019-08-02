/***************************************************************************************************
 * Copyright 2017 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/geometry/line.hpp>
#include <actl/iterator/output_type.hpp>

namespace ac::detail {

template <class Line, class It>
class scalar_to_point_adaptor {
public:
    using value_type = void;
    using difference_type = void;
    using pointer = void;
    using reference = void;
    using iterator_category = std::output_iterator_tag;

    explicit scalar_to_point_adaptor(const Line& line, It it) : it_{it}, line_{line} {}

    template <class T>
    void operator = (const T& x) {
        *it_ = line_(x);
    }

    scalar_to_point_adaptor& operator*() { return *this; }
    scalar_to_point_adaptor& operator++() { return *this; }
    scalar_to_point_adaptor operator++(int) { return *this; }

    operator It() const { return it_; }

private:
    It it_;
    const Line& line_;
};

template <class Line, class It>
inline auto adapt_iterator(const Line&, It it, scalar_tag) { return it; }

template <class Line, class It>
inline auto adapt_iterator(const Line& line, It it, point_tag) {
    return scalar_to_point_adaptor<Line, It>{line, it};
}

template <class Line, class Iterator>
inline auto adapt_iterator(const Line& line, Iterator it) {
    return adapt_iterator(line, it, geometry::tag_t<output_type_t<Iterator>>{});
}

}  // namespace ac::detail
