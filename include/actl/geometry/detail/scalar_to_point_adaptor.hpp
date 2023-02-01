// Copyright 2017 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/geometry/line.hpp>
#include <actl/range/iterator/facade/iterator_types.hpp>
#include <iterator>

namespace ac::detail {

template <class Line, class Iter>
class scalar_to_point_adaptor : public output_iterator_types
{
public:
    explicit scalar_to_point_adaptor(Line const& line, Iter iter)
        : line_{&line}, iter_{iter}
    {}

    scalar_to_point_adaptor& operator=(scalar_to_point_adaptor const&) =
        default;

    template <class T>
    void operator=(T const& x)
    {
        *iter_ = (*line_)(x);
    }

    scalar_to_point_adaptor& operator*()
    {
        return *this;
    }

    scalar_to_point_adaptor& operator++()
    {
        return *this;
    }

    scalar_to_point_adaptor operator++(int)
    {
        return *this;
    }

    operator Iter() const
    {
        return iter_;
    }

private:
    Line const* line_;
    Iter iter_;
};

} // namespace ac::detail
