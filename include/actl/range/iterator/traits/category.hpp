// Copyright 2021 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <iterator>

namespace ac {

// TODO: switch to iter_concept_t
template<typename T>
using iter_category_t = typename std::iterator_traits<T>::iterator_category;

} // namespace ac
