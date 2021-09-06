// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/assert.hpp>
#include <actl/container/array/static_array.hpp>
#include <actl/iterator/facade/iterator_facade.hpp>
#include <actl/range/facade/range_facade.hpp>

namespace ac {

template <class T, T... Values>
class semi_static_array;

namespace detail {

template <class T, T... Values>
struct ssa_types
{
    struct iter_types
    {
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type = T;
        using reference = T;
        using difference_type = use_default;
    };

    class iterator : public iterator_facade<iterator, iter_types>
    {
        friend class semi_static_array<T, Values...>;
        friend struct ac::iterator_core_access;

        explicit constexpr iterator(index i, const T* arr_iter) noexcept
            : i_{i}, arr_iter_{arr_iter}
        {}

        constexpr T get() const
        {
            return static_array<T, Values...>{}[i_];
        }

        constexpr T dereference() const
        {
            return get() == dynamic_size ? *arr_iter_ : get();
        }

        constexpr void increment()
        {
            if (get() == dynamic_size)
                ++arr_iter_;
            ++i_;
        }

        constexpr void decrement()
        {
            if (get() == dynamic_size)
                --arr_iter_;
            --i_;
        }

        constexpr bool equals(const iterator& rhs) const
        {
            return i_ == rhs.i_;
        }

        index i_;
        const T* arr_iter_;
    };

    using size_type = index;
};

} // namespace detail

template <class T, T... Values>
class semi_static_array
    : public range_facade<
          semi_static_array<T, Values...>,
          detail::ssa_types<T, Values...>>
{
    static constexpr size_t dynamic_count =
        (0 + ... + (Values == dynamic_size));
    using array_t = std::array<T, dynamic_count>;

    template <size_t I>
    static constexpr size_t dynamic_index()
    {
        size_t result = 0;
        for (size_t i = 0; i < I; ++i)
            result += size_t{
                static_array<T, Values...>{}[static_cast<index>(i)] ==
                dynamic_size};
        return result;
    }

    array_t a_;

    template <class Array, size_t... Is>
    constexpr auto equal_impl(
        const Array& rhs, std::index_sequence<Is...>) const noexcept
    {
        if constexpr (size() != Array::size())
            return std::false_type{};
        else
            return (... && ((*this)[Is] == rhs[Is]));
    }

public:
    using iterator = typename detail::ssa_types<T, Values...>::iterator;

    constexpr semi_static_array() noexcept = default;

    template <
        class... Ts,
        enable_int_if<
            ((sizeof...(Ts) == dynamic_count) && ... &&
             std::is_convertible_v<Ts, T>)> = 0>
    explicit constexpr semi_static_array(Ts&&... xs)
        : a_{std::forward<Ts>(xs)...}
    {}

    template <class R, enable_int_if<is_range_v<R>> = 0>
    explicit constexpr semi_static_array(R&& range)
    {
        auto iter = begin();
        for (auto&& x : range)
        {
            if (iter.get() == dynamic_size)
                a_[static_cast<size_t>(iter.arr_iter_ - a_.data())] = x;
            else
                ACTL_ASSERT(x == iter.get());
            ++iter;
        }
        ACTL_ASSERT(iter == end());
    }

    constexpr auto begin() const
    {
        return iterator{0, a_.data()};
    }

    constexpr auto end() const
    {
        return iterator{size(), a_.data() + a_.size()};
    }

    static constexpr index size() noexcept
    {
        return index{sizeof...(Values)};
    }

    constexpr T operator[](index i) const
    {
        auto it = begin();
        std::advance(it, i);
        return *it;
    }

    template <auto I>
    constexpr auto operator[](
        std::integral_constant<decltype(I), I> i) const noexcept
    {
        constexpr auto static_x = static_array<T, Values...>{}[i];
        if constexpr (static_x == dynamic_size)
            return a_[dynamic_index<I>()];
        else
            return static_x;
    }

    template <auto I>
    constexpr auto operator[](std::integral_constant<decltype(I), I> i) noexcept
        -> std::enable_if_t<static_array<T, Values...>{}[i] == dynamic_size, T&>
    {
        return a_[dynamic_index<I>()];
    }

    friend constexpr void swap(
        semi_static_array& lhs, semi_static_array& rhs) noexcept
    {
        lhs.a_.swap(rhs.a_);
    }

    template <class Array>
    friend constexpr auto operator==(
        const semi_static_array& lhs, const Array& rhs) noexcept
    {
        return lhs.equal_impl(rhs, std::make_index_sequence<size()>{});
    }

    template <class Array>
    friend constexpr auto operator!=(
        const semi_static_array& lhs, const Array& rhs) noexcept
    {
        return !(lhs == rhs);
    }
};

template <class T, T... Values>
struct range_traits<semi_static_array<T, Values...>> : default_range_traits
{
    static constexpr index static_size = sizeof...(Values);
};

} // namespace ac
