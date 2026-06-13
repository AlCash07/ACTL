// Copyright 2026 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/operation/operation/Operation.hpp>
#include <cstddef>

namespace ac {

/// Operation can implement the following member function to
/// specify argument indices that may be unused by the operation.
/// @code
/// static constexpr bool is_argument_maybe_unused(size_t index)
/// @endcode
///
/// Typical use cases are operations that select one of the arguments
/// or implement short circuiting.
///
/// By default, arguments that are expressions are automatically evaluated
/// before being passed to the operation (to simplify its implementation).
/// Maybe unused ones have to be manually evaluated by the operation instead.
///
/// This trait detects such arguments.
template<Operation Op, size_t ArgumentIndex>
struct is_argument_maybe_unused : std::false_type {};

template<Operation Op, size_t ArgumentIndex>
    requires requires { Op::is_argument_maybe_unused(ArgumentIndex); }
struct is_argument_maybe_unused<Op, ArgumentIndex> {
    static constexpr bool value = Op::is_argument_maybe_unused(ArgumentIndex);
};

} // namespace ac
