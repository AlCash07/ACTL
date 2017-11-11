/***************************************************************************************************
 * Copyright 2017 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/geometry/line.hpp>
#include <actl/iterator/iterator_adaptor.hpp>
#include <actl/type_traits/output_type.hpp>

namespace ac { namespace detail {

template <class Line, class It,
          class T = geometry::scalar_t<typename std::iterator_traits<It>::value_type>>
class scalar_to_point_adaptor
    : public iterator_adaptor<scalar_to_point_adaptor<Line, It>, It, use_default, T,
                              const scalar_to_point_adaptor<Line, It>&, T*> {
public:
    explicit scalar_to_point_adaptor(const Line& line, It it)
        : iterator_adaptor<scalar_to_point_adaptor<Line, It>, It, use_default, T,
                           const scalar_to_point_adaptor<Line, It>&, T*>(it),
          line_{line} {}

    void operator = (const T& value) const { *this->base() = line_(value); }

    operator It() const { return this->base(); }

private:
    friend struct ac::iterator_core_access;

    const scalar_to_point_adaptor& dereference() const { return *this; }

    const Line& line_;
};

template <class Line, class It>
inline auto adapt_iterator(const Line&, It it, scalar_tag) { return it; }

template <class Line, class It>
inline auto adapt_iterator(const Line& line, It it, point_tag) {
    return scalar_to_point_adaptor<Line, It>(line, it);
}

template <class Line, class Iterator>
inline auto adapt_iterator(const Line& line, Iterator it) {
    return adapt_iterator(line, it, geometry::tag_t<output_type_t<Iterator>>());
}

}}  // namespace ac::detail
