// Copyright 2022 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/meta/tuple.hpp>
#include <actl/range/traits/associated_types.hpp>

namespace ac {

namespace detail {

template <class T, class Seq = tuple_indices_t<T>>
struct matches_tuple;

template <class T, size_t... Is>
struct matches_tuple<T, std::index_sequence<Is...>> {
    static constexpr bool value =
        (... && std::is_same_v<range_value_t<T>, std::tuple_element_t<Is, T>>);
};

} // namespace detail

/// Range is strict if we can't deduce stricter types for its range elements if
/// we view it as a tuple. That is, if the range @p T is also a tuple,
/// then all the elements of this tuple are the same as range_value_t<T>.
/// An example is `std::array`.
/// This concept is useful in generic code where we need to decide whether
/// there's a benefit in treating a type as a Tuple instead of a Range.
template <class T>
concept StrictRange =
    Range<T> &&
    (!Tuple<T> || detail::matches_tuple<std::remove_reference_t<T>>::value);

} // namespace ac
