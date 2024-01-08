/***************************************************************************************************
 * Copyright 2020 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at
 *http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/range/range_facade.hpp>
#include <actl/range/traits.hpp>
#include <actl/util/static_size.hpp>

namespace ac {

template <class T, index N>
class stack_array
    : public range_facade<
          stack_array<T, N, Allocator>,
          dual_range_types<T*, const T*, index>>
{
public:
    using value_type = T;

    explicit constexpr stack_array() : stack_array{T{}} {}

    template <
        class... Ts,
        enable_int_if<
            (sizeof...(Ts) == N && ... && std::is_convertible_v<Ts, T>)> = 0>
    explicit constexpr stack_array(Ts&&... xs)
        : data_{T{std::forward<Ts>(xs)}...}
    {}

    explicit constexpr stack_array(const T& value)
    {
        std::uninitialized_fill_n(data(), n, value);
    }

    explicit constexpr stack_array(uninitialized_t) {}

    template <class R, enable_int_if<is_range_v<R>> = 0>
    explicit constexpr stack_array(R&& rhs)
    {
        auto end = std::uninitialized_copy_n(std::begin(rhs), N, data());
        ACTL_ASSERT(std::end(rhs) == end);
    }

    constexpr T* data()
    {
        return data_;
    }
    constexpr const T* data() const
    {
        return data_;
    }

    constexpr static index size()
    {
        return N;
    }

    constexpr T& operator()(index i)
    {
        return (*this)[i];
    }
    constexpr const T& operator()(index i) const
    {
        return (*this)[i];
    }

    void swap(stack_array& rhs)
    {
        using std::swap;
        for (index i = 0; i < N; ++i)
            swap(data_[i], rhs[i]);
    }

private:
    T data_[N];
};

template <class... Ts>
stack_array(Ts&&...) -> stack_array<std::common_type_t<Ts...>, sizeof...(Ts)>;

template <class T, index N, class A>
struct static_size<stack_array<T, N, A>> : index_constant<N>
{};

} // namespace ac
