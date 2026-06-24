// Copyright 2019 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/functional/noexcept/AC_DEDUCE_NOEXCEPT_AND_RETURN.hpp>
#include <actl/operation/operation/Operation.hpp>
#include <actl/operation/operation/inout.hpp>
#include <actl/operation/policy/tuned_operation.hpp>
#include <actl/operation/resolver/all.hpp>

namespace ac {

/// Base class for user-defined operations.
///
/// Uses CRTP idiom https://en.cppreference.com/w/cpp/language/crtp.html
/// to provide function call operator implementation, which:
/// - Creates an expression without evaluating it by default.
/// - If one of the arguments is ac::inout then
///   evaluates the expression into it.
template<typename DerivedOperation>
struct operation_base {
    /// Applies operation to arguments by creating an expression
    /// which stores operation and arguments:
    /// - if passed as r-value, then by value after a move
    ///   (otherwise we'd immediately get a dangling reference).
    /// - if passed as l-value, then by reference or by value
    ///   making a copy if the copy is cheap.
    ///   In either case, the arguments are never modified
    ///   unless wrapped into ac::out or ac::inout.
    template<typename... Args>
    constexpr auto operator()(Args&&... args) const& //
        AC_DEDUCE_NOEXCEPT_AND_RETURN(               //
            expression{this->derived(), std::forward<Args>(args)...}
        )
    template<typename... Args>
        constexpr auto operator()(Args&&... args) &&
        AC_DEDUCE_NOEXCEPT_AND_RETURN( //
            expression{std::move(*this).derived(), std::forward<Args>(args)...}
        )

    /// Evaluates operation immediately into the ac::inout argument,
    /// also using it as one of the inputs.
    ///
    /// ac::inout argument can be at any position as long as
    /// it's compatible with the operation result type.
    ///
    /// @return Reference stored in the ac::inout argument.
    template<typename... Args>
        requires(... || is_inout_v<Args>)
    constexpr decltype(auto) operator()(Args&&... args) const& {
        static_assert(
            1 == (... + is_inout_v<Args>), "single inout argument expected"
        );
        auto&& op = resolve_operation<DerivedOperation, Args...>(derived());
        auto& target = find_target(args...);
        op.evaluate_to(out{target}, remove_inout(args)...);
        return target;
    }

    constexpr DerivedOperation const& derived() const& noexcept {
        return static_cast<DerivedOperation const&>(*this);
    }
    constexpr DerivedOperation&& derived() && noexcept {
        return static_cast<DerivedOperation&&>(*this);
    }

    template<typename Target, typename... Args>
    constexpr void evaluate_to(out<Target&> target, Args const&... args) const
        requires requires { *target = this->derived().evaluate(args...); }
    {
        *target = this->derived().evaluate(args...);
    }

    struct enable_operators;
};

} // namespace ac
