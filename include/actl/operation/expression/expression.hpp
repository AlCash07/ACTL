// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/operation/expression/argument_traits.hpp>
#include <actl/operation/expression/evaluate.hpp>
#include <actl/operation/expression/result_type.hpp>

namespace ac {

template<typename Result>
struct can_convert_expression_implicitly : std::false_type {};

template<Operation Op, typename... Args>
class expression : public expression_data_t<Op, Args...> {
    using base_t = expression_data_t<Op, Args...>;

public:
    using base_t::base_t;

    constexpr operator resolved_result_type_t<Op, Args...>() const
        requires(can_convert_expression_implicitly<
                 resolved_result_type_t<Op, Args...>>::value)
    {
        return eval(*this);
    }

    struct enable_operators;
};

template<typename... Ts>
expression(Ts&&...) -> expression<value_if_small<Ts>...>;

template<typename... Ts>
struct is_expression<expression<Ts...>> : std::true_type {};

template<typename... Ts>
struct expression_result_type<expression<Ts...>> {
    using type = resolved_result_type_t<Ts...>;
};

} // namespace ac
