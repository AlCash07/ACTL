/***************************************************************************************************
 * Copyright 2019 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/assert.hpp>
#include <actl/range/traits.hpp>
#include <actl/util/static_size.hpp>

namespace ac {

namespace detail {

template <index N>
class span_size {
public:
    static_assert(N > 0);

    constexpr span_size([[maybe_unused]] index n) { ACTL_ASSERT(n == N); }

    static constexpr index size() { return N; }
};

template <>
class span_size<0> {
public:
    constexpr span_size() = default;
    constexpr span_size([[maybe_unused]] index n) { ACTL_ASSERT(n == 0); }

    static constexpr index size() { return 0; }
};

template <>
class span_size<dynamic_size> {
public:
    constexpr span_size() = default;
    constexpr span_size(index n) : size_{n} { ACTL_ASSERT(n >= 0); }

    constexpr index size() const { return size_; }

private:
    index size_ = 0;
};

}  // namespace detail

template <class T, index N = dynamic_size>
class span : detail::span_size<N> {
    using base_t = detail::span_size<N>;

public:
    using element_type    = T;
    using value_type      = std::remove_cv_t<T>;
    using index_type      = index;
    using difference_type = index;
    using pointer         = T*;
    using reference       = T&;
    using iterator        = pointer;
    using const_iterator  = iterator;

    static constexpr index extent = N;

    constexpr span() = default;

    constexpr span(T* ptr, index count) : base_t{count}, data_{ptr} {}

    constexpr span(T* first, T* last) : span{first, last - first} {}

    template <class Range, enable_int_if<is_contiguous_range_v<std::remove_reference_t<Range>>> = 0>
    constexpr span(Range&& r) : span{std::data(r), static_cast<index>(std::size(r))} {}

    constexpr T* begin() const { return data(); }

    constexpr T* end() const { return data() + size(); }

    constexpr T& front() const { return data()[0]; }

    constexpr T& back() const { return data()[size() - 1]; }

    constexpr T& operator[](index i) const {
        ACTL_ASSERT(data() && 0 <= i && i < size());
        return data()[i];
    }

    constexpr T* data() const { return data_; }

    using base_t::size;

    constexpr bool empty() const { return size() == 0; }

    constexpr span<T> first(index n) const {
        ACTL_ASSERT(0 <= n && n <= size());
        return {begin(), n};
    }

    constexpr span<T> last(index n) const {
        ACTL_ASSERT(0 <= n && n <= size());
        return {end() - n, n};
    }

private:
    T* data_ = nullptr;
};

template <class Range, class R = std::remove_reference_t<Range>>
span(Range&&) -> span<std::remove_pointer_t<pointer_t<R>>, static_size_v<R>>;

template <class T, index N>
struct static_size<span<T, N>> : index_constant<N> {};

template <class T, index N = dynamic_size>
using cspan = span<const T, N>;

template <class S, class T>
struct is_span : std::false_type {};

template <class T, index N>
struct is_span<span<T, N>, T> : std::true_type {};

}  // namespace ac
