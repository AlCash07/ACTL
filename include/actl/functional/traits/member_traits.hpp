// Copyright 2025 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/functional/traits/detail/modify_class.hpp>

namespace ac {

/// Class parameter of the member function including qualifiers.
template<MemberFunction MF>
using class_parameter_t = at_t<parameters_t<MF>, 0>;

/// Class parameter of the member function excluding qualifiers.
template<MemberFunction MF>
using unqualified_class_t = std::remove_cvref_t<at_t<parameters_t<MF>, 0>>;

} // namespace ac
