// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/category/iterator.hpp>
#include <actl/meta/dependent.hpp>
#include <actl/utility/use_default.hpp>

namespace ac::detail {

template <class Iter, bool HasReverse = is_bidirectional_iterator_v<Iter>>
struct range_ts {
    using value_type = value_type_t<Iter>;
    using reference = reference_t<Iter>;
    using pointer = pointer_t<Iter>;
    using difference_type = difference_type_t<Iter>;
    using iterator = Iter;
};

template <class Iter>
struct range_ts<Iter, true> : range_ts<Iter, false> {
    using reverse_iterator = std::reverse_iterator<Iter>;
};

template <class CIter, bool HasReverse = is_bidirectional_iterator_v<CIter>>
struct crange_ts {
    using const_reference = reference_t<CIter>;
    using const_pointer = pointer_t<CIter>;
    using const_iterator = CIter;
};

template <class CIter>
struct crange_ts<CIter, true> : crange_ts<CIter, false> {
    using const_reverse_iterator = std::reverse_iterator<CIter>;
};

template <class T, class = void>
struct range_types : range_ts<typename T::iterator> {
    using typename range_ts<typename T::iterator>::difference_type;
    using size_type = deduce_t<typename T::size_type, std::make_unsigned_t<difference_type>>;
};

template <class T>
struct range_types<T, std::void_t<typename T::const_iterator>>
    : range_types<T, int>
    , crange_ts<typename T::const_iterator> {};

} // namespace ac::detail
