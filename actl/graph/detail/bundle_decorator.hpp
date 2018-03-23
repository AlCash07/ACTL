/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <type_traits>

namespace ac::detail {

// Special compressed_pair that decorates T with bundled property B.
template <class T, class B, bool = std::is_empty_v<B> && !std::is_final_v<B>>
class bundle_decorator : public T, private B {
public:
    template <class T0, class... Ts>
    explicit constexpr bundle_decorator(T0&& arg0, Ts&&... args)
        : T(std::forward<T0>(arg0)), B(std::forward<Ts>(args)...) {}

    B&       bundle()       { return *this; }
    const B& bundle() const { return *this; }
};

template <class T, class B>
class bundle_decorator<T, B, false> : public T {
public:
    template <class T0, class... Ts>
    explicit constexpr bundle_decorator(T0&& arg0, Ts&&... args)
        : T(std::forward<T0>(arg0)), bundle_(std::forward<Ts>(args)...) {}

    B&       bundle()       { return bundle_; }
    const B& bundle() const { return bundle_; }

private:
    B bundle_;
};

}  // namespace ac::detail
