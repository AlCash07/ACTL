// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/functional/parameter/out.hpp>
#include <actl/operation/expression/argument_traits.hpp>
#include <actl/operation/expression/expression_storage.hpp>
#include <actl/operation/expression/result_type.hpp>
#include <actl/operation/operation/Operation.hpp>
#include <actl/operation/overload/resolve_overload.hpp>

namespace ac {

template<typename Result>
struct can_convert_expression_implicitly : std::false_type {};

template<typename Operation, typename... Args>
class expression : public expression_storage_t<Operation, Args...> {
    using base_t = expression_storage_t<Operation, Args...>;

public:
    using base_t::base_t;

    constexpr operator resolved_result_type_t<Operation, Args...>() const
        requires(can_convert_expression_implicitly<
                 resolved_result_type_t<Operation, Args...>>::value)
    {
        return eval(*this);
    }

    struct enable_operators;
};

template<typename... Ts>
expression(Ts&&...) -> expression<value_if_small<Ts>...>;

template<typename T>
struct is_expression : std::false_type {};

template<typename... Ts>
struct is_expression<expression<Ts...>> : std::true_type {};

template<typename T>
inline constexpr bool is_expression_v =
    is_expression<std::remove_cvref_t<T>>::value;

template<typename... Ts>
struct expression_result_type<expression<Ts...>> {
    using type = resolved_result_type_t<Ts...>;
};

template<typename Target, typename Operation, size_t... Is, typename... Args>
constexpr void assign(
    out<Target>& target,
    expression_storage<Operation, std::index_sequence<Is...>, Args...> const&
        expr
) {
    auto&& op = resolve_overload<Args...>(default_context{}, expr.operation);
    op.evaluate_to(target, std::get<Is>(expr.arguments)...);
}

} // namespace ac
