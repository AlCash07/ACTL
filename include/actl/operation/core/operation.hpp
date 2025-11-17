// Copyright 2019 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/operation/core/inout.hpp>
#include <actl/operation/overload/resolve_expression.hpp>
#include <actl/operation/overload/resolve_overload.hpp>
#include <actl/operation/policy/resolve_overload.hpp>

namespace ac {

template<typename Derived>
struct operation {
    using operation_category = operation_tag;

    constexpr Derived const& derived() const& noexcept {
        return static_cast<Derived const&>(*this);
    }
    constexpr Derived&& derived() && noexcept {
        return static_cast<Derived&&>(*this);
    }

    // Passing arguments to an operation by default
    // doesn't evaluate it but only creates an expression.
    // That's unless one of the arguments is inout.
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

    struct enable_operators;
};

} // namespace ac
