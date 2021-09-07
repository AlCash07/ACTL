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

        constexpr T get() const noexcept
        {
            return static_array<T, Values...>{}[i_];
        }

        constexpr T dereference() const noexcept
        {
            return get() == dynamic_size ? *arr_iter_ : get();
        }

        constexpr void increment() noexcept
        {
            if (get() == dynamic_size)
                ++arr_iter_;
            ++i_;
        }

        constexpr void decrement() noexcept
        {
            if (get() == dynamic_size)
                --arr_iter_;
            --i_;
        }

        constexpr bool equals(const iterator& rhs) const noexcept
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

    array_t array_;

    template <size_t... Is>
    static constexpr size_t dymanic_index_impl(
        std::index_sequence<Is...>) noexcept
    {
        return (
            0 + ... +
            size_t{static_array<T, Values...>{}[index{Is}] == dynamic_size});
    }

    template <size_t I>
    static constexpr size_t dynamic_index =
        dymanic_index_impl(std::make_index_sequence<I>{});

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
        size_t N = sizeof...(Ts),
        enable_int_if<
            ((N != dynamic_count && N == sizeof...(Values)) && ... &&
             std::is_convertible_v<Ts, T>)> = 0>
    explicit constexpr semi_static_array(Ts... xs) noexcept(
        ACTL_ASSERT_IS_NOEXCEPT())
        : semi_static_array{std::array<T, sizeof...(Values)>{xs...}}
    {}

    template <
        class R,
        enable_int_if<
            is_range_v<R> &&
            !std::is_same_v<remove_cvref_t<R>, std::array<T, dynamic_count>>> =
            0>
    explicit constexpr semi_static_array(R&& range) noexcept(
        ACTL_ASSERT_IS_NOEXCEPT())
        : array_{} // default initialization is needed here to support constexpr
    {
        auto iter = begin();
        for (auto&& x : range)
        {
            if (iter.get() == dynamic_size)
                array_[static_cast<size_t>(iter.arr_iter_ - array_.data())] = x;
            else
                ACTL_ASSERT(x == iter.get());
            ++iter;
        }
        ACTL_ASSERT(iter == end());
    }

    template <
        class... Ts,
        enable_int_if<
            ((sizeof...(Ts) == dynamic_count) && ... &&
             std::is_convertible_v<Ts, T>)> = 0>
    explicit constexpr semi_static_array(Ts... xs) noexcept : array_{xs...}
    {}

    explicit constexpr semi_static_array(
        const std::array<T, dynamic_count>& a) noexcept
        : array_{a}
    {}

    constexpr auto begin() const noexcept
    {
        return iterator{0, array_.data()};
    }

    constexpr auto end() const noexcept
    {
        return iterator{size(), array_.data() + array_.size()};
    }

    static constexpr index size() noexcept
    {
        return index{sizeof...(Values)};
    }

    constexpr T operator[](index i) const noexcept
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
            return array_[dynamic_index<I>];
        else
            return static_x;
    }

    template <auto I>
    constexpr auto operator[](std::integral_constant<decltype(I), I> i) noexcept
        -> std::enable_if_t<static_array<T, Values...>{}[i] == dynamic_size, T&>
    {
        return array_[dynamic_index<I>];
    }

    friend constexpr void swap(
        semi_static_array& lhs, semi_static_array& rhs) noexcept
    {
        lhs.array_.swap(rhs.array_);
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
