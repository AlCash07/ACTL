// Copyright 2022 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/meta/concepts/core/default_initializable.hpp>
#include <actl/meta/concepts/object/copyable.hpp>

namespace ac {

template <class T>
concept TriviallySemiregular =
    std::semiregular<T> && TriviallyCopyable<T> &&
    TriviallyDefaultInitializable<T> &&
    // Trivial type is defined as trivially copyable and default constructible
    // (which we check above), but it can have a different internal
    // implementation by a compiler, so it's useful to do an extra check.
    std::is_trivial_v<T>;

template <class T>
concept NothrowSemiregular =
    std::semiregular<T> && NothrowCopyable<T> && NothrowDefaultInitializable<T>;

} // namespace ac
