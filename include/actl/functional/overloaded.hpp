// Copyright 2024 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt)

#pragma once

namespace ac {

/// Function object that combines all the overloads from the given Lambdas.
///
/// Designed primarily for convenience when visiting a variant,
/// see https://en.cppreference.com/w/cpp/utility/variant/visit
/// @code
/// std::visit(ac::overloaded{
///     [](int i) { std::print("int = {}", i); },
///     [](std::string_view s) { std::print("string = “{}”", s); },
/// }, std::variant<int, std::string>{...});
/// @endcode
template<class... Lambdas>
struct overloaded : Lambdas... {
    using Lambdas::operator()...;
};

// The link above says this deduction guide is not needed as of C++20,
// but in fact for some compilers it is needed.
template<class... Lambdas>
overloaded(Lambdas...) -> overloaded<Lambdas...>;

} // namespace ac
