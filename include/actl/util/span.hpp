/***************************************************************************************************
 * Copyright 2019 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <iterator>
#include <type_traits>

namespace ac {

template <class T>
class span {
public:
    using element_type = T;
    using value_type = std::remove_cv_t<T>;
    using index_type = std::ptrdiff_t;
    using difference_type = std::ptrdiff_t;
    using pointer = T*;
    using reference = T&;
    using iterator = pointer;

    constexpr span() = default;

    constexpr span(T* ptr, index_type count) : data_{ptr}, size_{count} {}

    constexpr span(T* first, T* last) : span{first, last - first} {}

    template <class Range>
    constexpr span(Range& r) : span{std::data(r), static_cast<index_type>(std::size(r))} {}

    constexpr T* begin() const { return data(); }

    constexpr T* end() const { return data() + size(); }

    constexpr T& back() const { return data()[size() - 1]; }

    constexpr T& operator[](index_type i) const { return data()[i]; }

    constexpr T* data() const { return data_; }

    constexpr index_type size() const { return size_; }

    constexpr bool empty() const { return size() == 0; }

private:
    T* data_ = nullptr;
    index_type size_ = 0;
};

template <class Range>
span(Range&) -> span<std::remove_pointer_t<decltype(std::data(std::declval<Range&>()))>>;

}  // namespace ac
