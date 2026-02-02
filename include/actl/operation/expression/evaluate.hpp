// Copyright 2021 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/functional/parameter/out.hpp>
#include <actl/operation/expression/expression_data.hpp>
#include <actl/operation/overload/resolve_overload.hpp>

namespace ac {

/// Evaluates the expression.
///
/// If the passed value is not an expression then its reference
/// is forwarded with value category preserved, without making a copy.
///
/// @note The name is chosen as abbreviated "eval" instead of "evaluate",
/// because it can be used often and a shorter name is helpful.
/// Also, it's consistent with C++ libraries that support lazy evaluation:
/// - Eigen:
///   https://libeigen.gitlab.io/eigen/docs-nightly/classEigen_1_1DenseBase.html#aa73e57a2f0f7cfcb4ad4d55ea0b6414b
/// - xtensor:
///   https://xtensor.readthedocs.io/en/latest/expression.html#forcing-evaluation
/// As well as scripting languages:
/// - Python:
///   https://docs.python.org/3/library/functions.html#eval
/// - JavaScript:
///   https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/eval
template<typename T>
    requires(!is_expression_v<T>)
constexpr decltype(auto) eval(T&& x) {
    return std::forward<T>(x);
}

namespace detail {

template<typename Op, size_t ArgumentIndex, typename Args>
constexpr decltype(auto) argument_at(const Args& args) {
    using RawOp = std::remove_reference_t<Op>;
    if constexpr (RawOp::is_argument_maybe_unused(ArgumentIndex))
        return std::get<ArgumentIndex>(args);
    else
        return eval(std::get<ArgumentIndex>(args));
}

} // namespace detail

template<Operation Op, size_t... Is, typename... Args>
constexpr decltype(auto) eval(
    expression_data<Op, std::index_sequence<Is...>, Args...> const& expression
) {
    auto&& operation =
        resolve_overload<decltype(eval(std::declval<Args const&>()))...>(
            expression.operation
        );
    return operation.evaluate(
        detail::argument_at<decltype(operation), Is>(expression.arguments)...
    );
}

template<typename Target, Operation Op, size_t... Is, typename... Args>
constexpr void assign(
    out<Target>& target,
    expression_data<Op, std::index_sequence<Is...>, Args...> const& expression
) {
    auto&& operation = resolve_overload<Args...>(expression.operation);
    operation.evaluate_to(
        out{target},
        detail::argument_at<decltype(operation), Is>(expression.arguments)...
    );
}

} // namespace ac
