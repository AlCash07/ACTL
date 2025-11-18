// Copyright 2025 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/memory/AC_NO_UNIQUE_ADDRESS.hpp>
#include <tuple>

namespace ac {

// Expression has different methods depending whether it's an operation or not.
// To avoid copy-pasting the arguments storage, we prefer
// to inherit from expression_storage over using CRTP to add methods,
// because pros outweight the cons.
// Pros:
// - CRTP adds complexity with casting to the derived class.
// Cons:
// - Explicit `using` for the constructor in derived classes.
// - During debugging, arguments might not be immediately visible,
//   because they are stored in a separate base class.
template<typename Operation, typename... Args>
struct expression_storage {
    static constexpr size_t argument_count = sizeof...(Args);

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

} // namespace ac
