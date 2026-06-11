// Copyright 2021 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/functional/parameter/out.hpp>
#include <actl/operation/expression/expression_data.hpp>
#include <actl/operation/resolver/operation_resolver.hpp>

namespace ac {

template<typename T, typename... Args>
struct Eval {
    static constexpr decltype(auto) call(T&& t, Args&&...) {
        return std::forward<T>(t);
    }
};

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
template<typename T, typename... Args>
constexpr decltype(auto) eval(T&& t, Args&&... args) {
    return Eval<T, Args...>::call(
        std::forward<T>(t), std::forward<Args>(args)...
    );
}

template<typename T, typename... Args>
using result_t = decltype(eval(std::declval<T>(), std::declval<Args>()...));

namespace detail {

template<bool IsLazy, typename T>
struct PrepareArgument {
    static constexpr decltype(auto) call(T&& t) {
        return std::forward<T>(t);
    }
};
template<typename T>
struct PrepareArgument<false, T> : Eval<T> {};

template<Operation Op, typename Indices, typename... Args>
struct EvalOperation;

template<Operation Op, size_t... Is, typename... Args>
struct EvalOperation<Op, std::index_sequence<Is...>, Args...> {
    static constexpr decltype(auto) call(Op&& op, Args&&... args) {
        auto&& resolved_op =
            resolve_operation<Op, result_t<Args>...>(std::forward<Op>(op));
        using RawOp = std::remove_reference_t<decltype(resolved_op)>;
        return resolved_op.evaluate(
            PrepareArgument<RawOp::is_argument_maybe_unused(Is), Args>::call(
                std::forward<Args>(args)
            )...
        );
    }
};

} // namespace detail

template<Operation Op, typename... Args>
struct Eval<Op, Args...>
    : detail::EvalOperation<Op, std::index_sequence_for<Args...>, Args...> {};

} // namespace ac
