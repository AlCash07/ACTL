// Copyright 2017 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/geometry/algorithm/crosses/crosses.hpp>
#include <actl/geometry/algorithm/intersect/line_line_2d.hpp>
#include <actl/iterator/facade/iterator_facade.hpp>

namespace ac {

namespace detail {

class flag_output_iterator
    : public iterator_facade<flag_output_iterator, std::output_iterator_tag>
{
public:
    bool flag = false;

    template <class T>
    void operator=(T) const
    {}

    flag_output_iterator const& operator*() const noexcept
    {
        return *this;
    }

private:
    friend struct ac::iterator_core_access;

    void increment()
    {
        flag = true;
    }
};

} // namespace detail

template <class Policy, class T0, class K0, class T1, class K1>
bool crosses(
    Policy const& policy,
    line<T0, 2, K0> const& lhs,
    line<T1, 2, K1> const& rhs)
{
    return intersect(policy, lhs, rhs, detail::flag_output_iterator{}).flag;
}

template <class Policy, class T0, class K0, class T1, class K1>
bool crosses(
    general_position_policy<Policy> gpp,
    line<T0, 2, K0> const& lhs,
    line<T1, 2, K1> const& rhs)
{
    return crosses(line_scalar_policy{gpp.policy}, lhs, rhs);
}

} // namespace ac
