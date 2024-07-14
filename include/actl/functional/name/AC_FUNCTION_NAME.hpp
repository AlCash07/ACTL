// Copyright 2024 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt)

#pragma once

/// String literal representing portable short name
/// of the enclosing function (or a function template),
/// that is without any namespaces, qualifiers and parameters.
#define AC_FUNCTION_NAME __func__

// TODO: implement AC_FUNCTION_FULL_NAME
