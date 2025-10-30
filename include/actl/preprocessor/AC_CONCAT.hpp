// Copyright 2017 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

/// Concatenates arguments after they have been expanded.
/// Analog of BOOST_PP_CAT.
#define AC_CONCAT(l, r) AC_CONCAT_IMPL(l, r)

#define AC_CONCAT_IMPL(l, r) l##r
