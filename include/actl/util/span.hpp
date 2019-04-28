/***************************************************************************************************
 * Copyright 2019 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/assert.hpp>
#include <actl/types.hpp>
#include <iterator>
#include <type_traits>

namespace ac {

template <class T>
class span {
public:
    using element_type = T;
    using value_type = std::remove_cv_t<T>;
    using index_type = index;
    using difference_type = index;
    using pointer = T*;
    using reference = T&;
    using iterator = pointer;

    constexpr span() = default;

    constexpr span(T* ptr, index count) : data_{ptr}, size_{count} { ACTL_ASSERT(count >= 0); }

    constexpr span(T* first, T* last) : span{first, last - first} {}

    template <class Range>
    constexpr span(Range& r) : span{std::data(r), static_cast<index>(std::size(r))} {}

    constexpr T* begin() const { return data(); }

    constexpr T* end() const { return data() + size(); }

    constexpr T& back() const {
        ACTL_ASSERT(data() && !empty());
        return data()[size() - 1];
    }

    constexpr T& operator[](index i) const {
        ACTL_ASSERT(data() && 0 <= i && i < size());
        return data()[i];
    }

    constexpr T* data() const { return data_; }

    constexpr index size() const { return size_; }

    constexpr bool empty() const { return size() == 0; }

private:
    T* data_ = nullptr;
    index size_ = 0;
};

template <class Range>
span(Range&) -> span<std::remove_pointer_t<decltype(std::data(std::declval<Range&>()))>>;

}  // namespace ac
