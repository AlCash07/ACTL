// Copyright 2022 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/range/traits/concepts.hpp>
#include <actl/range/traits/super_range.hpp>
#include <type_traits>

namespace ac {

namespace detail {

template<typename T>
concept HasKeyType = requires { typename T::key_type; };

template<typename T>
concept HasMappedType = requires { typename T::mapped_type; };

} // namespace detail

template<typename T>
concept AssociativeRange =
    Range<T> && (detail::HasKeyType<std::remove_reference_t<T>> ||
                 detail::HasKeyType<super_range_t<std::remove_reference_t<T>>>);

template<typename T>
concept PairAssociativeRange =
    AssociativeRange<T> &&
    (detail::HasMappedType<std::remove_reference_t<T>> ||
     detail::HasMappedType<super_range_t<std::remove_reference_t<T>>>);

template<typename T>
concept SimpleAssociativeRange =
    AssociativeRange<T> && !PairAssociativeRange<T>;

template<typename C>
concept SequenceRange = Range<C> && !AssociativeRange<C>;

} // namespace ac
