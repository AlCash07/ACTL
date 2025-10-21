// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

namespace ac {

/// Concept of a function object, that is a type with
/// a non-overloaded `operator()`.
template<class T>
concept FunctionObject = requires(T) {
    &T::operator();
    // TODO: investigate whether type qualifiers on Fn can make it
    // impossible to call `operator()`, and, on the other hand, may
    // disambiguate it.
};

} // namespace ac
