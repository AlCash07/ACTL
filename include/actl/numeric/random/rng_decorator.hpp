/***************************************************************************************************
 * Copyright 2018 Roman Rizvanov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/types.hpp>
#include <random>
#include <type_traits>

namespace ac {

template <class Rng>
struct rng_decorator : public Rng {
    explicit constexpr rng_decorator() = default;

    template <class Seed>
    explicit rng_decorator(Seed&& s) {
        seed(std::forward<Seed>(s));
    }

    /**
     * Generates uniform random number in the range [from, to).
     */
    template <class T, class = std::enable_if_t<std::is_arithmetic_v<T>>>
    T uniform(T from, T to) {
        if constexpr (std::is_integral_v<T>) {
            return std::uniform_int_distribution<T>{from, to - 1}(*this);
        } else {
            return std::uniform_real_distribution<T>{from, to}(*this);
        }
    }

    /**
     * Generates uniform random number in the range [0, to).
     */
    template <class T>
    T uniform(T to) {
        return uniform(T{}, to);
    }

    /**
     * Generates normal (Gaussian) random number with mean = @p mean and stddev = @p stddev.
     */
    template <class T = double, class = std::enable_if_t<std::is_floating_point_v<T>>>
    T normal(T mean = T{}, T stddev = T{1}) {
        return std::normal_distribution<T>{mean, stddev}(*this);
    }

    void seed(czstring value) {
        Rng& base = static_cast<Rng&>(*this);
        Rng default_rng;
        do {
            default_rng();
            base = default_rng;
            for (auto i = value; *i; ++i) {
                this->state[0] += static_cast<typename Rng::result_type>(*i);
            }
        } while (base() == base());  // improbable unless RNG state is degenerate
    }
};

}  // namespace ac