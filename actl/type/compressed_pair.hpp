/***************************************************************************************************
 * Copyright 2017 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <type_traits>

namespace ac {

namespace detail {

template <int I, class T, bool = std::is_empty_v<T> && !std::is_final_v<T>>
class member : private T {
protected:
    constexpr member() = default;

    template <class U>
    constexpr member(U&& value) : T(std::forward<U>(value)) {}

    constexpr T&       get()       noexcept { return *this; }
    constexpr const T& get() const noexcept { return *this; }
};

template <int I, class T>
class member<I, T, false> {
protected:
    constexpr member() = default;

    template <class U>
    constexpr member(U&& value) : value_{std::forward<U>(value)} {}

    constexpr T&       get()       noexcept { return value_; }
    constexpr const T& get() const noexcept { return value_; }

private:
    T value_;
};

}  // namespace detail

/**
 * Pair that utilizes empty-base optimization in case one of the elements is empty.
 * Reference: http://talesofcpp.fusionfenix.com/post-18/episode-ten-when-size-does-matter
 */
template <class T1, class T2>
class compressed_pair : private detail::member<0, T1>, private detail::member<1, T2> {
public:
    using first_type  = T1;
    using second_type = T2;

    explicit constexpr compressed_pair() = default;

    template <class U1, class U2>
    explicit constexpr compressed_pair(U1&& first, U2&& second)
        : detail::member<0, T1>(std::forward<U1>(first)),
          detail::member<1, T2>(std::forward<U2>(second)) {}

    constexpr T1&       first()       noexcept { return detail::member<0, T1>::get(); }
    constexpr const T1& first() const noexcept { return detail::member<0, T1>::get(); }

    constexpr T2&       second()       noexcept { return detail::member<1, T2>::get(); }
    constexpr const T2& second() const noexcept { return detail::member<1, T2>::get(); }
};

template <class Device, class T1, class T2>
inline bool read(Device& input, compressed_pair<T1, T2>& arg) {
    return read(input, arg.first(), arg.second());
}

template <class Device, class T1, class T2>
inline int write(Device& output, const compressed_pair<T1, T2>& arg) {
    return write(output, arg.first(), arg.second());
}

}  // namespace ac
