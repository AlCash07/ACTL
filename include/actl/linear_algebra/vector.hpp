// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/range/range_facade.hpp>
#include <actl/range/traits.hpp>
#include <actl/util/static_size.hpp>
// #include <actl/functional/composite/range.hpp>
#include <algorithm>

namespace ac {
namespace math {

#if 0
/**
 * N-dimensional vector.
 */
template <class T, index N, class Allocator = std::allocator<T>>
class vec {
public:
    using value_type = T;

    constexpr T& operator()(index i) { return (*this)[i]; }

    void swap(vec& rhs) {
        using std::swap;
    }
};

template <class... Ts>
vec(Ts&&...) -> vec<std::common_type_t<Ts...>, sizeof...(Ts)>;

template <class T>
using vec2 = vec<T, 2>;

template <class T>
using vec3 = vec<T, 3>;

// template <class T>
// using vecX = vec<T, -1>;

struct vector_tag : range_tag {};

template <class T>
struct category_impl<T, std::enable_if_t<is_contiguous_range_v<T>>> {
    using type = vector_tag;
};

template <index Depth>
struct Size : operation<Size<Depth>> {
    template <class T>
    static constexpr auto perform(const T& x) {
        if constexpr (detail::type_depth_v<T> < Depth) {
            return none{};
        } else if constexpr (static_size_v<T> == dynamic_size) {
            return std::size(x);
        } else {
            return index_constant<static_size_v<T>>;
        }
    }
};
template <index Depth = 0>
inline constexpr Size<Depth> size;

template <index Depth, class Policy, class Op, size_t... Is, class... Ts>
inline auto perform_policy(Size<Depth> op, const Policy& policy,
                           const expression<Op, std::index_sequence<Is...>, Ts...>& e) {
    return common(op(std::get<Is>(e.args)...));
}

template <class Derived>
class vec_base {
public:
    constexpr auto size() const { return eval(size(derived())); }

    constexpr decltype(auto) operator[](index i) const { return eval(subscript(i)(derived())); }
    constexpr decltype(auto) operator()(index i) const { return (*this)[i]; }

private:
    const Derived& derived() const { return static_cast<const Derived&>(*this); }
};

struct vectorized {
    template <class Op, class... Ts>
    static constexpr auto evaluate(const Op& op, const Ts&... xs) {
        constexpr index max_depth = max(detail::type_depth_v<Ts>...);
        auto n = eval(common(size<max_depth>(xs)...));
        vec<> dst(n);
        evaluate_to(op, dst, xs...);
        return dst;
    }

    template <class Op, class T, class... Ts>
    static constexpr void evaluate_to(const Op& op, T& dst, const Ts&... xs) {
        auto n = size(dst);
        for (index i = 0; i < n; ++i) {
            op(out(dst[i]), xs[i]...);
        }
    }
};

template <class Policy, index N, class T0, class T1>
inline constexpr auto dot(const Policy& policy, const vec<T0, N>& lhs, const vec<T1, N>& rhs) {
    math::result_t<math::Mul, Policy, T0, T1> res = 0;
    for (index i = 0; i < N; ++i) res += product(policy, lhs[i], rhs[i]);
    return res;
}

template <class Policy, index N, class T, enable_int_if<is_policy_v<Policy>> = 0>
inline constexpr auto dot(const Policy& policy, const vec<T, N>& p) {
    return dot(policy, p, p);
}
#endif

} // namespace math

// template <class T, index N, class A>
// struct static_size<math::vec<T, N, A>> : index_constant<N> {};

} // namespace ac
