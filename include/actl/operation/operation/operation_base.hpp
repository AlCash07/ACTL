// Copyright 2019 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/operation/operation/Operation.hpp>
#include <actl/operation/operation/inout.hpp>
#include <actl/operation/overload/resolve_expression.hpp>
#include <actl/operation/overload/resolve_overload.hpp>
#include <actl/operation/policy/resolve_overload.hpp>

namespace ac {

/// Base class for user-defined operations.
///
/// Uses CRTP idiom https://en.cppreference.com/w/cpp/language/crtp.html
/// to provide function call operator implementation, which:
/// - Creates an expression without evaluating it by default.
/// - If one of the arguments is inout then
///   evaluates the expression into it.
template<typename DerivedOperation>
struct operation_base {
    template<typename... Args>
    constexpr auto operator()(Args&&... args) const& {
        return expression{derived(), std::forward<Args>(args)...};
    }
    template<typename... Args>
    constexpr auto operator()(Args&&... args) && {
        return expression{
            std::move(*this).derived(), std::forward<Args>(args)...
        };
    }

    template<typename... Args>
        requires(... || is_inout_v<Args>)
    constexpr decltype(auto) operator()(Args&&... args) const& {
        static_assert(
            1 == (... + is_inout_v<Args>), "single inout argument expected"
        );
        auto&& op = resolve_overload<Args...>(default_context{}, derived());
        auto& target = find_target(args...);
        op.evaluate_to(target, remove_inout(args)...);
        return target;
    }

    constexpr DerivedOperation const& derived() const& noexcept {
        return static_cast<DerivedOperation const&>(*this);
    }
    constexpr DerivedOperation&& derived() && noexcept {
        return static_cast<DerivedOperation&&>(*this);
    }

    struct enable_operators;
};

} // namespace ac
