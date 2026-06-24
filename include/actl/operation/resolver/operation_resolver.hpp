// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/operation/expression/raw.hpp>
#include <actl/sequence/type_array/type_array.hpp>

namespace ac {

struct identity_resolver {
    struct is_resolved;

    template<typename T, typename... Policies>
    constexpr T operator()(T&& t, Policies const&...) const {
        return std::forward<T>(t);
    }
};

template<Operation Op, typename Args, typename Indices, typename... Policies>
struct constant_resolver;

template<Operation Op, typename ArgsArray, typename... Policies>
struct specialization_resolver : identity_resolver {};

template<Operation Op, typename ArgsArray, typename... Policies>
struct policy_resolver : specialization_resolver<Op, ArgsArray, Policies...> {};

template<typename Op, typename ArgsArray, typename... Policies>
struct operation_resolver : policy_resolver<Op, ArgsArray, Policies...> {};

template<typename Op, typename... Args>
inline constexpr auto resolve_operation =
    operation_resolver<raw_t<Op>, type_array<raw_t<Args>...>>{};

template<typename Op, typename ArgsArray, typename... Policies>
inline constexpr bool is_operation_resolved_v = requires {
    typename operation_resolver<raw_t<Op>, ArgsArray, Policies...>::is_resolved;
};

// Operation expressions can contain leaf nodes with values
// instead of operations. Their resolution is handled here.
template<typename T, typename ArgsArray, typename... Policies>
    requires(!Operation<T>)
struct operation_resolver<T, ArgsArray, Policies...> : identity_resolver {};

} // namespace ac
