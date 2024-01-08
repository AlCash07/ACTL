// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/category/range.hpp>
#include <actl/operation/core/composite_operation.hpp>
#include <actl/operation/core/type_operation.hpp>
#include <actl/operation/scalar/comparison/equal.hpp>
#include <algorithm>

namespace ac {

namespace ranges {

struct equal_f : operation<equal_f>
{
    template <class... Ts>
    using result_type = bool;

    template <class T, class U, class Equal>
    bool evaluate(const T& lhs, const U& rhs, const Equal& equal_element) const
    {
        return true;
        // return std::equal(std::begin(lhs), std::end(lhs), std::begin(rhs),
        // std::end(rhs), strip_placeholders(equal_element));
    }

    template <class T, class U, class Equal = ac::equal_f>
    constexpr auto operator()(
        T&& lhs, U&& rhs, Equal&& equal_element = {}) const
    {
        return operation<equal_f>::operator()(
            std::forward<T>(lhs),
            std::forward<U>(rhs),
            std::forward<Equal>(equal_element)(
                value_type_f<T>, value_type_f<U>));
    }
    // };

    // template <class T, class U>
    // constexpr auto operator()(T&& lhs, U&& rhs) const {
    //     return (*this)(std::forward<T>(lhs), std::forward<U>(rhs),
    //     ac::equal);
    // }

    // template <class T, class U, class Equal>
    // constexpr auto operator()(T&& lhs, U&& rhs, Equal&& equal_element) const
    // {
    //     return operation<equal_f>::operator()(std::forward<T>(lhs),
    //     std::forward<U>(rhs),
    //         std::forward<Equal>(equal_element)(value_type_f<T>,
    //         value_type_f<U>));
    // }
};
inline constexpr equal_f equal;

} // namespace ranges

template <class T, class U>
struct overload<equal_f, range_tag, T, U>
{
    static constexpr auto formula = ranges::equal;
};

} // namespace ac
