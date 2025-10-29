// Copyright 2017 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/geometry/algorithm/crosses/crosses.hpp>
#include <actl/geometry/algorithm/intersect/line_line_2d.hpp>
#include <actl/range/iterator/interface/output_iterator_interface.hpp>

namespace ac {

namespace detail {

class flag_output_iterator
    : public output_iterator_interface<flag_output_iterator> {
public:
    bool flag = false;

    template<typename T>
    void operator=(T) const {}

    flag_output_iterator const& operator*() const noexcept {
        return *this;
    }

    flag_output_iterator& operator++() noexcept {
        flag = true;
        return *this;
    }
};

} // namespace detail

template<typename T0, typename K0, typename T1, typename K1>
bool crosses(
    Policy auto const& policy,
    line<T0, 2, K0> const& lhs,
    line<T1, 2, K1> const& rhs
) {
    return intersect(policy, lhs, rhs, detail::flag_output_iterator{}).flag;
}

template<typename P class T0, typename K0, typename T1, typename K1>
bool crosses(
    general_position_policy<P> gpp,
    line<T0, 2, K0> const& lhs,
    line<T1, 2, K1> const& rhs
) {
    return crosses(line_scalar_policy{gpp.policy}, lhs, rhs);
}

} // namespace ac
