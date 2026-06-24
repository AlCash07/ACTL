// Copyright 2026 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/numeric/constant.hpp>
#include <actl/operation/arg.hpp>
#include <actl/operation/resolver/operation_resolver.hpp>
#include <utility> // for std::index_sequence

namespace ac {

template<Operation Op, typename... Args, size_t... Is, typename... Policies>
struct constant_resolver<
    Op,
    type_array<Args...>,
    std::index_sequence<Is...>,
    Policies...> {
    template<size_t Index, typename Arg>
    static constexpr auto unwrapped_arg() noexcept {
        if constexpr (is_constant_v<Arg>)
            return Arg::value;
        else
            return arg<Index, sizeof...(Args)>;
    }

    template<typename Op1>
    constexpr auto operator()(Op1&& op, Policies const&... policies) const {
        using resolver = operation_resolver<
            Op,
            type_array<unwrap_constant_t<Args>...>,
            Policies...>;
        return resolver{}(std::forward<Op1>(op), policies...)(
            unwrapped_arg<Is, Args>()...
        );
    }
};

} // namespace ac
