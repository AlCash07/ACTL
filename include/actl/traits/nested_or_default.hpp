// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <type_traits>

namespace ac {

#define NESTED_OR_DEFAULT(nested)                                             \
    template <class T, class Default, class = void>                           \
    struct nested##_or_default {                                              \
        using type = Default;                                                 \
    };                                                                        \
    template <class T, class Default>                                         \
    struct nested##_or_default<T, Default, std::void_t<typename T::nested>> { \
        using type = typename T::nested;                                      \
    };                                                                        \
    template <class T, class Default>                                         \
    using nested##_or_default_t = typename nested##_or_default<T, Default>::type;

NESTED_OR_DEFAULT(iterator_category)
NESTED_OR_DEFAULT(value_type)
NESTED_OR_DEFAULT(reference)
NESTED_OR_DEFAULT(difference_type)
NESTED_OR_DEFAULT(size_type)

#undef NESTED_OR_DEFAULT

}  // namespace ac
