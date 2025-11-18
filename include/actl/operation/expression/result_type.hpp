// Copyright 2021 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/operation/operation/operation_traits.hpp>
#include <actl/operation/overload/resolve_overload.hpp>

namespace ac {

template<typename Op, typename... Args>
struct result_type {
    using type = typename Op::template result_type<Args...>;
};

template<typename Op, typename... Args>
    requires requires(Op op, Args... args) { op.evaluate(args...); }
struct result_type<Op, Args...> {
    using type = decltype(std::declval<Op>().evaluate(std::declval<Args>()...));
};

template<typename T>
struct expression_result_type {
    using type = T;
};

template<bool, typename Op, typename... Args>
struct resolved_result_type1 : result_type<std::remove_cvref_t<Op>, Args...> {};

template<typename Op, typename... Args>
struct resolved_result_type1<false, Op, Args...>
    : expression_result_type<
          decltype(resolve_overload<
                   Args...>(default_context{}, std::declval<Op>())(
              std::declval<Args>()...
          ))> {};

template<typename... Ts>
using resolved_result_type = resolved_result_type1<
    is_overload_resolved_v<default_context, Ts...>,
    Ts...>;

template<typename... Ts>
using resolved_result_type_t = typename resolved_result_type<Ts...>::type;

} // namespace ac
