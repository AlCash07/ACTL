// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/assert.hpp>
#include <actl/container/array/static_array.hpp>
#include <actl/container/extent.hpp>
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

        explicit constexpr iterator(size_t i, const T* arr_iter) noexcept
            : i_{i}, arr_iter_{arr_iter}
        {}

        constexpr T get() const noexcept
        {
            return static_array<T, Values...>{}[i_];
        }

        constexpr T dereference() const noexcept
        {
            return get() == dynamic_extent<T> ? *arr_iter_ : get();
        }

        constexpr void increment() noexcept
        {
            if (get() == dynamic_extent<T>)
                ++arr_iter_;
            ++i_;
        }

        constexpr void decrement() noexcept
        {
            if (get() == dynamic_extent<T>)
                --arr_iter_;
            --i_;
        }

        constexpr bool equals(const iterator& rhs) const noexcept
        {
            return i_ == rhs.i_;
        }

        size_t i_;
        const T* arr_iter_;
    };

    using size_type = size_t;
};

} // namespace detail

template <class T, T... Values>
class semi_static_array
    : public range_facade<
          semi_static_array<T, Values...>,
          detail::ssa_types<T, Values...>>
{
    template <class... Ts>
    static constexpr bool can_construct_from()
    {
        if constexpr (sizeof...(Ts) == sizeof...(Values))
            return (... && can_convert_v<extent_holder_t<Values>, Ts>);
        else
            return false;
    }

public:
    static constexpr auto static_values = static_array<T, Values...>{};

    static constexpr auto size_dynamic()
    {
        return size_constant<(0 + ... + size_t{Values == dynamic_extent<T>})>{};
    }

    static_assert(size_dynamic() > 0);

    using iterator = typename detail::ssa_types<T, Values...>::iterator;

    constexpr semi_static_array() noexcept = default;

    template <
        class T0,
        class... Ts,
        size_t N = sizeof...(Ts),
        enable_int_if<
            sizeof...(Ts) + 1 != size_dynamic() &&
            can_construct_from<T0, Ts...>()> = 0>
    explicit constexpr semi_static_array(T0 x0, Ts... xs) noexcept(
        ACTL_ASSERT_IS_NOEXCEPT())
        : semi_static_array{indexes, x0, xs...}
    {}

    // The first parameter is needed because of the bug in std::is_trivial impl.
    template <
        class... Ts,
        enable_int_if<
            sizeof...(Ts) + 1 == size_dynamic() &&
            (... && std::is_convertible_v<Ts, T>)> = 0>
    explicit constexpr semi_static_array(T x, Ts... xs) noexcept
        : array_{x, static_cast<T>(xs)...}
    {}

    constexpr auto begin() const noexcept
    {
        return iterator{0, array_.data()};
    }

    constexpr auto end() const noexcept
    {
        return iterator{size(), array_.data() + array_.size()};
    }

    static constexpr auto size() noexcept
    {
        return size_constant<sizeof...(Values)>{};
    }

    constexpr T operator[](size_t i) const noexcept
    {
        auto it = begin();
        std::advance(it, i);
        return *it;
    }

    template <auto I>
    constexpr auto operator[](
        std::integral_constant<decltype(I), I> i) const noexcept
    {
        if constexpr (static_values[i] != dynamic_extent<T>)
            return static_values[i];
        else
            return array_[dynamic_index<I>];
    }

    template <class U, U I>
    constexpr auto operator[](std::integral_constant<U, I>) noexcept
        -> std::enable_if_t<static_values[I] == dynamic_extent<T>, T&>
    {
        return array_[dynamic_index<I>];
    }

    friend constexpr void swap(
        semi_static_array& lhs, semi_static_array& rhs) noexcept
    {
        lhs.array_.swap(rhs.array_);
    }

    friend constexpr auto operator==(
        const semi_static_array& lhs, const semi_static_array& rhs) noexcept
    {
        for (size_t i = 0; i != size_dynamic(); ++i)
            if (lhs.array_[i] != rhs.array_[i])
                return false;
        return true;
    }

    friend constexpr auto operator!=(
        const semi_static_array& lhs, const semi_static_array& rhs) noexcept
    {
        return !(lhs == rhs);
    }

private:
    using array_t = std::array<T, size_dynamic()>;

    array_t array_;

    static constexpr auto indexes = std::make_index_sequence<size()>{};

    template <size_t... Is>
    static constexpr size_t dymanic_index_impl(
        std::index_sequence<Is...>) noexcept
    {
        return (0 + ... + size_t{static_values[Is] == dynamic_extent<T>});
    }

    template <size_t I>
    static constexpr size_t dynamic_index =
        dymanic_index_impl(std::make_index_sequence<I>{});

    template <size_t I>
    constexpr void assign_at([[maybe_unused]] T x) noexcept(
        ACTL_ASSERT_IS_NOEXCEPT())
    {
        if constexpr (static_values[I] == dynamic_extent<T>)
            array_[dynamic_index<I>] = x;
    }

    template <size_t... Is, class... Ts>
    explicit constexpr semi_static_array(
        std::index_sequence<Is...>,
        Ts... xs) noexcept(ACTL_ASSERT_IS_NOEXCEPT())
        : array_{}
    {
        (..., assign_at<Is>(convert<extent_holder_t<static_values[Is]>>(xs)));
    }
};

template <class T, T... Values>
struct range_traits<semi_static_array<T, Values...>> : default_range_traits
{
    static constexpr index static_size = sizeof...(Values);
};

} // namespace ac
