/***************************************************************************************************
 * Copyright 2017 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/hash.hpp>

namespace ac {

namespace detail {

template <int I, class T, bool = std::is_empty_v<T> && !std::is_final_v<T>>
class member : private T {
protected:
    template <class... Ts>
    constexpr member(Ts&&... args) : T(std::forward<Ts>(args)...) {}

    constexpr T&       get() noexcept { return *this; }
    constexpr const T& get() const noexcept { return *this; }
};

template <int I, class T>
class member<I, T, false> {
protected:
    template <class... Ts>
    constexpr member(Ts&&... args) : value_(std::forward<Ts>(args)...) {}

    constexpr T&       get() noexcept { return value_; }
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
class compressed_pair : private detail::member<1, T1>, private detail::member<2, T2> {
public:
    using first_type  = T1;
    using second_type = T2;

    explicit constexpr compressed_pair() = default;

    template <class T, class... Ts>
    explicit constexpr compressed_pair(T&& first, Ts&&... second)
        : detail::member<1, T1>(std::forward<T>(first))
        , detail::member<2, T2>(std::forward<Ts>(second)...) {}

    constexpr T1&       first() noexcept { return detail::member<1, T1>::get(); }
    constexpr const T1& first() const noexcept { return detail::member<1, T1>::get(); }

    constexpr T2&       second() noexcept { return detail::member<2, T2>::get(); }
    constexpr const T2& second() const noexcept { return detail::member<2, T2>::get(); }
};

template <class T1, class T2>
inline bool operator == (const compressed_pair<T1, T2>& lhs, const compressed_pair<T1, T2>& rhs) {
    return lhs.first() == rhs.first() && lhs.second() == rhs.second();
}

template <class T1, class T2>
inline bool operator < (const compressed_pair<T1, T2>& lhs, const compressed_pair<T1, T2>& rhs) {
    return lhs.first() < rhs.first() || (lhs.first() == rhs.first() && lhs.second() < rhs.second());
}

template <class T1, class T2>
inline size_t hash_value(const compressed_pair<T1, T2>& arg) {
    return hash_value(arg.first(), arg.second());
}

template <class Device, class T1, class T2>
inline bool read(Device& input, compressed_pair<T1, T2>& arg) {
    return read(input, arg.first(), arg.second());
}

template <class Device, class T1, class T2>
inline int write(Device& output, const compressed_pair<T1, T2>& arg) {
    return write(output, arg.first(), arg.second());
}

}  // namespace ac

SPECIALIZE_STD_VALUE_HASH(ac::compressed_pair)
