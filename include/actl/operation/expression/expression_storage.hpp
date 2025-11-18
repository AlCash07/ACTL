// Copyright 2025 Oleksandr Bacherikov.
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
// to inherit from expression_storage over using CRTP to add methods,
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
template<typename Operation, typename ArgIndices, typename... Args>
class expression_storage;

template<typename Operation, size_t... Indices, typename... Args>
class expression_storage<Operation, std::index_sequence<Indices...>, Args...> {
public:
    static constexpr size_t argument_count = sizeof...(Args);
    static_assert(argument_count == sizeof...(Indices));

    AC_NO_UNIQUE_ADDRESS Operation operation;
    // TODO: avoid std::tuple because of slow compilation time
    // and move constructor never being trivial caused by a wording defect.
    AC_NO_UNIQUE_ADDRESS std::tuple<Args...> arguments;

    expression_storage() = default;

    template<typename Op, typename... Ts>
        requires(std::is_constructible_v<Operation, Op &&> &&
                 sizeof...(Ts) == argument_count &&
                 (... && std::is_constructible_v<Args, Ts>))
    constexpr expression_storage(Op&& op, Ts&&... args)
        : operation{std::forward<Op>(op)}
        , arguments{std::forward<Ts>(args)...} {}
};

template<typename Operation, typename... Args>
using expression_storage_t =
    expression_storage<Operation, std::index_sequence_for<Args...>, Args...>;

} // namespace ac
