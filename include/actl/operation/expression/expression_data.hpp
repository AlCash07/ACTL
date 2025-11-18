// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/memory/AC_NO_UNIQUE_ADDRESS.hpp>
#include <tuple>
#include <utility> // for std::index_sequence_for

namespace ac {

// Expression has different methods depending whether it's an operation or not.
//
// To avoid copy-pasting the arguments storage, we prefer
// to inherit from expression_data over using CRTP to add methods,
// because pros outweight the cons.
// Pros:
// - CRTP adds complexity with casting to the derived class.
// - index_sequence needed to expand the arguments tuple
//   can be included into this type without making
//   the expression type more verbose,
//   which would make compilation errors harder to read.
// Cons:
// - Explicit `using` for the constructor in derived classes.
// - During debugging, arguments might not be immediately visible,
//   because they are stored in a separate base class.
//
// Args must be last, because they are a variadic pack.
template<Operation Op, typename ArgIndices, typename... Args>
class expression_data;

template<Operation Op, size_t... Indices, typename... Args>
class expression_data<Op, std::index_sequence<Indices...>, Args...> {
public:
    static constexpr size_t argument_count = sizeof...(Args);
    static_assert(argument_count == sizeof...(Indices));

    AC_NO_UNIQUE_ADDRESS Op operation;
    // TODO: avoid std::tuple because of slow compilation time
    // and move constructor never being trivial caused by a wording defect.
    AC_NO_UNIQUE_ADDRESS std::tuple<Args...> arguments;

    expression_data() = default;

    template<typename OpT, typename... ArgsT>
        requires(std::is_constructible_v<Op, OpT &&> &&
                 sizeof...(ArgsT) == argument_count &&
                 (... && std::is_constructible_v<Args, ArgsT>))
    constexpr expression_data(OpT&& op, ArgsT&&... args)
        : operation{std::forward<OpT>(op)}
        , arguments{std::forward<ArgsT>(args)...} {}
};

template<Operation Op, typename... Args>
using expression_data_t =
    expression_data<Op, std::index_sequence_for<Args...>, Args...>;

template<typename T>
struct is_expression : std::false_type {};

template<typename T>
inline constexpr bool is_expression_v =
    is_expression<std::remove_cvref_t<T>>::value;

} // namespace ac
