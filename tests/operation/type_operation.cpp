// Copyright 2026 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt)

#include <actl/numeric/arithmetic/additive/all.hpp>
#include <actl/operation/type_operation.hpp>

/* strip_placeholders replaces placeholders with associated Arg operations */
constexpr ac::Placeholder<int, ac::Arg<0, 1>> raw_placeholder;
static_assert(
    std::is_same_v<ac::Arg<0, 1>, decltype(strip_placeholders(raw_placeholder))>
);
static_assert(std::is_same_v<int&&, decltype(ac::strip_placeholders(0))>);

constexpr auto expression_with_placeholders =
    ac::add(1, ac::Placeholder<int, ac::Arg<0, 2>>{}) -
    ac::Placeholder<int, ac::Arg<1, 2>>{};
static_assert(std::is_same_v<
              decltype(1 + ac::l_ - ac::r_),
              decltype(strip_placeholders(expression_with_placeholders))>);

/* type_operation is resolved into a placeholder
 * and allows to resolve operations using types derived from arguments */
template<typename Arg>
using SignedType = ac::TypeOperation<std::make_signed, Arg>;

using resolved_operation_t =
    decltype(ac::resolve_operation<SignedType<ac::Arg<0, 1>>, unsigned int>(
        SignedType<ac::Arg<0, 1>>{}
    ));
static_assert(
    std::is_same_v<ac::Placeholder<int, ac::Arg<0, 1>>, resolved_operation_t>
);

constexpr auto input_expression =
    SignedType<ac::Arg<0, 2>>{} + SignedType<ac::Arg<1, 2>>{};
using resolved_expression_t = decltype( //
    ac::resolve_operation<
        decltype(input_expression),
        unsigned int,
        unsigned int>(input_expression)
);
static_assert( //
    std::is_same_v<
        decltype(ac::add_integer(
            ac::Placeholder<int, ac::Arg<0, 2>>{},
            ac::Placeholder<int, ac::Arg<1, 2>>{}
        )),
        resolved_expression_t>
);
