// Copyright 2025 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

namespace ac {

/// Concept of a type that can be called as a function with the given Args,
/// that is as `fn(args...)`.
///
/// Not all the types that are considered to be functions satisfy this concept.
/// In particular, member functions are called using special syntax
/// @code
/// (instance.*member)(args...);
/// @endcode
///
/// C++ provides `std::invoke` as a unified way to call all the functions.
/// However, we highly recommend not to use it and instead wrap member functions
/// into `std::mem_fn` when they need to be called in a unified way.
/// That's because `std::invoke` adds 1 or 2 nested function calls for
/// every function (not just member functions that are rarely called this way),
/// which in debug mode require more manual actions to step into,
/// and overall decrease performance if not inlined.
/// This problem isn't as critical because it's specific to debug builds.
/// But there's a simple `std::mem_fn` solution to avoid it completely.
template<typename Function, typename... Args>
concept Callable = requires(Function fn, Args... args) { fn(args...); };

} // namespace ac
