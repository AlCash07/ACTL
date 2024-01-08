// Copyright 2021 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/category/range.hpp>
#include <actl/category/utility/is_subcategory_of.hpp>
#include <actl/operation/core/operation_expression.hpp>

namespace ac {

template <class T>
struct placeholder
{};

template <class T>
struct raw<placeholder<T>> : raw<T>
{};

template <class T>
struct is_placeholder : std::false_type
{};

template <class T>
struct is_placeholder<placeholder<T>> : std::true_type
{};

template <class T>
constexpr T eval(placeholder<T>)
{
    return T{};
}

struct type_operation_tag
{
    using base = operation_tag;
};

template <class T>
struct type_f_t : operation<type_f_t<T>>
{
    using category = type_operation_tag;
    using argument_tag = unclassified_tag;

    template <class... Ts>
    constexpr auto evaluate(const Ts&... xs) const
        -> decltype(expand_expression(std::declval<T>(), xs...));
    //  {
    //     return {};
    // }
};
template <class T>
inline constexpr type_f_t<std::remove_reference_t<T>> type_f;
// template <class T, class U = std::remove_reference_t<T>>
// inline constexpr std::conditional_t<is_operation_v<U>, type_f_t<U>,
// placeholder<U>> type_f;

template <class T>
struct raw<type_f_t<T>> : raw<T>
{};

template <class T>
struct value_type_f_t : operation<value_type_f_t<T>>
{
    using category = type_operation_tag;
    using argument_tag = range_tag;

    template <class... Ts>
    constexpr auto evaluate(const Ts&... xs) const
        -> value_type_t<decltype(expand_expression(std::declval<T>(), xs...))>;
    //  {
    //     return {};
    // }
};
template <class T, class U = std::remove_reference_t<T>>
inline constexpr std::conditional_t<
    is_operation_v<U>,
    value_type_f_t<U>,
    type_f_t<value_type_t<U>>>
    value_type_f;

template <class... Ts>
inline constexpr bool are_placeholders_v =
    (... && (is_placeholder<Ts>::value ||
             is_subcategory_of_v<category_t<Ts>, type_operation_tag>));

template <class T>
constexpr decltype(auto) strip_placeholders(T&& x)
{
    return std::forward<T>(x);
}

template <class Op, class... Ts, enable_int_if<are_placeholders_v<Ts...>> = 0>
constexpr decltype(auto) strip_placeholders(const expression<Op, Ts...>& oe)
{
    return oe.operation();
}

} // namespace ac
