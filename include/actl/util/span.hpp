/***************************************************************************************************
 * Copyright 2019 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/container/size_holder.hpp>
#include <actl/range/traits.hpp>

namespace ac {

template <class T, index N = dynamic_size>
class span {
public:
    using element_type    = T;
    using value_type      = std::remove_cv_t<T>;
    using index_type      = index;
    using difference_type = index;
    using pointer         = T*;
    using reference       = T&;
    using iterator        = pointer;

    static constexpr index extent = N;

    template <index M = N, enable_int_if<M <= 0> = 0>
    constexpr span() : storage_{nullptr, 0} {}

    constexpr span(T* ptr, index count) : storage_{ptr, count} {}

    constexpr span(T* first, T* last) : span{first, last - first} {}

    template <class Range, enable_int_if<is_contiguous_range_v<Range>> = 0>
    constexpr span(Range&& r) : span{std::data(r), static_cast<index>(std::size(r))} {}

    constexpr T* begin() const { return data(); }

    constexpr T* end() const { return data() + size(); }

    constexpr T& front() const { return data()[0]; }

    constexpr T& back() const { return data()[size() - 1]; }

    constexpr T& operator[](index i) const {
        ACTL_ASSERT(0 <= i && i < size());
        return data()[i];
    }

    constexpr T* data() const { return storage_.data; }

    constexpr index size() const { return storage_.size(); }

    constexpr bool empty() const { return size() == 0; }

    constexpr span<T> first(index n) const {
        ACTL_ASSERT(0 <= n && n <= size());
        return {begin(), n};
    }

    constexpr span<T> last(index n) const {
        ACTL_ASSERT(0 <= n && n <= size());
        return {end() - n, n};
    }

    constexpr span<T> subspan(index offset, index count) const {
        ACTL_ASSERT(0 <= offset && 0 <= count && offset + count <= size());
        return {data() + offset, count};
    }

    constexpr span<T> subspan(index offset) const { return subspan(offset, size() - offset); }

private:
    struct storage_t : size_holder<N> {
        T* data;
        constexpr storage_t(T* ptr, index count) : size_holder<N>{count}, data{ptr} {
            ACTL_ASSERT(ptr || count == 0);
        }
    };

    storage_t storage_;
};

template <class Range, enable_int_if<is_contiguous_range_v<Range>> = 0>
span(Range &&) -> span<std::remove_pointer_t<decltype(std::data(std::declval<Range>()))>,
                       static_size_v<std::remove_reference_t<Range>>>;

template <class T, index N>
struct static_size<span<T, N>> : index_constant<N> {};

template <class T, index N = dynamic_size>
using cspan = span<const T, N>;

template <class S, class T>
struct is_span : std::false_type {};

template <class T, index N>
struct is_span<span<T, N>, T> : std::true_type {};

}  // namespace ac
