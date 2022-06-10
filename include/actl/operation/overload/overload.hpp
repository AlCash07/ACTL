// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/category/category.hpp>

namespace ac {

template <class Op, class = void>
struct default_overload
{};

template <class Op>
struct default_overload<Op, std::void_t<decltype(Op::formula)>>
{
    static constexpr auto formula = Op::formula;
};

template <class Void, class Op, class... Ts>
struct overload : default_overload<Op>
{};

} // namespace ac
