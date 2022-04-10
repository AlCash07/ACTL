// Copyright 2019 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/category/range.hpp>
#include <actl/meta/static_size.hpp>
#include <actl/range/interface/contiguous_range_interface.hpp>
#include <actl/range/traits.hpp>
#include <actl/range/utility/size_holder.hpp>

namespace ac {

namespace detail {

template <class T>
struct span_types
{
    using iterator = T*;
    using size_type = size_t;
};

} // namespace detail

template <class T, size_t N = dynamic_size>
class span
    : public contiguous_range_interface<span<T, N>, detail::span_types<T>>
{
public:
    using element_type = T;

    static constexpr size_t extent = N;

    template <size_t M = N, enable_int_if<M == 0 || M == dynamic_size> = 0>
    constexpr span() : storage_{nullptr, 0}
    {}

    constexpr span(T* ptr, size_t count) : storage_{ptr, count} {}

    constexpr span(T* first, T* last)
        : span{first, static_cast<size_t>(last - first)}
    {}

    template <class Range, enable_int_if<is_contiguous_range_v<Range>> = 0>
    constexpr span(Range&& r) : span{std::data(r), std::size(r)}
    {}

    constexpr T* data() const
    {
        return storage_.data;
    }

    constexpr size_t size() const
    {
        return storage_.size();
    }

    constexpr span<T> first(size_t n) const
    {
        ACTL_ASSERT(n <= size());
        return {data(), n};
    }

    constexpr span<T> last(size_t n) const
    {
        ACTL_ASSERT(n <= size());
        return {this->end() - n, n};
    }

    constexpr span<T> subspan(size_t offset, size_t count) const
    {
        ACTL_ASSERT(offset + count <= size());
        return {data() + offset, count};
    }

    constexpr span<T> subspan(size_t offset) const
    {
        return subspan(offset, static_cast<size_t>(size() - offset));
    }

private:
    struct storage_t : size_holder<N>
    {
        T* data;
        constexpr storage_t(T* ptr, size_t count)
            : size_holder<N>{count}, data{ptr}
        {
            ACTL_ASSERT(ptr || count == 0);
        }
    };

    storage_t storage_;
};

template <class Range, enable_int_if<is_contiguous_range_v<Range>> = 0>
span(Range&&) -> span<
    std::remove_pointer_t<decltype(std::data(std::declval<Range>()))>,
    static_size_v<std::remove_reference_t<Range>>>;

template <class T, size_t N>
struct range_traits<span<T, N>> : default_range_traits
{};

template <class T, size_t N = dynamic_size>
using cspan = span<const T, N>;

template <class S, class T>
struct is_span : std::false_type
{};

template <class T, index N>
struct is_span<span<T, N>, T> : std::true_type
{};

} // namespace ac
