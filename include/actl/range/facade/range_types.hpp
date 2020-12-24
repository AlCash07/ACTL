// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/iterator/traits.hpp>
#include <actl/util/use_default.hpp>

namespace ac {

template <class Iterator, class Size = use_default,
          bool HasReverse = is_bidirectional_iterator_v<Iterator>>
struct range_types {
    using value_type = value_type_t<Iterator>;
    using reference = reference_t<Iterator>;
    using pointer = pointer_t<Iterator>;
    using difference_type = difference_t<Iterator>;
    using iterator = Iterator;
    using size_type = deduce_t<Size, std::make_unsigned_t<difference_type>>;
};

template <class It, class S>
struct range_types<It, S, true> : range_types<It, S, false> {
    using reverse_iterator = std::reverse_iterator<It>;
};

template <class Iterator, class ConstIterator, class Size = use_default,
          bool HasReverse = is_bidirectional_iterator_v<Iterator>>
struct dual_range_types : range_types<Iterator, Size> {
    using const_reference = reference_t<ConstIterator>;
    using const_pointer = pointer_t<ConstIterator>;
    using const_iterator = ConstIterator;
};

template <class It, class CIt, class S>
struct dual_range_types<It, CIt, S, true> : dual_range_types<It, CIt, S, false> {
    using const_reverse_iterator = std::reverse_iterator<CIt>;
};

}  // namespace ac
