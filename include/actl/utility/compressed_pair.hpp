// Copyright 2017 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/functional/operators.hpp>
#include <actl/io/tuple.hpp>
#include <actl/operation/tuple/comparison.hpp>
#include <actl/utility/introspection.hpp>

namespace ac {

template <class T, bool = std::is_empty_v<T> && !std::is_final_v<T>>
class ebo : private T {
public:
    template <class... Ts>
    explicit constexpr ebo(Ts&&... args) : T{std::forward<Ts>(args)...} {}

    constexpr T& get() noexcept {
        return *this;
    }
    constexpr const T& get() const noexcept {
        return *this;
    }
};

template <class T>
class ebo<T, false> {
public:
    template <class... Ts>
    explicit constexpr ebo(Ts&&... args) : value_{std::forward<Ts>(args)...} {}

    constexpr T& get() noexcept {
        return value_;
    }
    constexpr const T& get() const noexcept {
        return value_;
    }

private:
    T value_;
};

namespace detail {

template <class T>
struct cpb1 : operators::base<ebo<T>> {
    using operators::base<ebo<T>>::base;
};

template <class T>
struct cpb2 : ebo<T> {
    using ebo<T>::ebo;
};

}  // namespace detail

/// Pair that utilizes empty-base optimization in case one of the elements is empty.
/// Reference: http://talesofcpp.fusionfenix.com/post-18/episode-ten-when-size-does-matter
template <class T1, class T2>
class compressed_pair : private detail::cpb1<T1>, private detail::cpb2<T2> {
public:
    using first_type = T1;
    using second_type = T2;

    explicit constexpr compressed_pair() = default;

    template <class T, class... Ts>
    explicit constexpr compressed_pair(T&& first, Ts&&... second)
        : detail::cpb1<T1>{std::forward<T>(first)}, detail::cpb2<T2>{std::forward<Ts>(second)...} {}

    constexpr compressed_pair(const compressed_pair&) = default;
    constexpr compressed_pair& operator=(const compressed_pair&) = default;

    constexpr compressed_pair(compressed_pair&&) = default;
    constexpr compressed_pair& operator=(compressed_pair&&) = default;

    constexpr T1& first() noexcept {
        return detail::cpb1<T1>::get();
    }
    constexpr const T1& first() const noexcept {
        return detail::cpb1<T1>::get();
    }

    constexpr T2& second() noexcept {
        return detail::cpb2<T2>::get();
    }
    constexpr const T2& second() const noexcept {
        return detail::cpb2<T2>::get();
    }

private:
    DEFINE_HASH(first(), second())
};

template <size_t I, class T1, class T2>
auto& get(const compressed_pair<T1, T2>& p) {
    if constexpr (I == 0) {
        return p.first();
    } else {
        static_assert(I == 1);
        return p.second();
    }
}

template <class T1, class T2>
auto operator==(const compressed_pair<T1, T2>& lhs, const compressed_pair<T1, T2>& rhs) {
    return equal(lhs.first(), rhs.first()) && equal(lhs.second(), rhs.second());
}

template <class T1, class T2>
auto operator<(const compressed_pair<T1, T2>& lhs, const compressed_pair<T1, T2>& rhs) {
    return less(lhs, rhs);
}

}  // namespace ac

namespace std {

template <class T1, class T2>
struct tuple_size<ac::compressed_pair<T1, T2>> {
    static constexpr size_t value = 2;
};

template <class T1, class T2>
struct tuple_element<0, ac::compressed_pair<T1, T2>> {
    using type = T1;
};

template <class T1, class T2>
struct tuple_element<1, ac::compressed_pair<T1, T2>> {
    using type = T2;
};

}  // namespace std
