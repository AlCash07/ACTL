// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/io/argument/function.hpp>
#include <actl/io/utility/predicate.hpp>
#include <actl/string/char_classes.hpp>

namespace ac::io {

template <class T>
struct skip : private predicate<T>
{
    explicit constexpr skip(T value) : predicate<T>{value} {}

    bool operator()(Device auto& id) const
    {
        while (!id.eof() && predicate<T>::operator()(id.peek()))
            id.move(1);
        return true;
    }
};

inline constexpr auto ws = skip{is_space}; // skip whitespace

} // namespace ac::io
