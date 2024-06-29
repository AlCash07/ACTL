// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/map/composite_map.hpp>
#include <actl/map/member_map.hpp>
#include <actl/memory/no_unique_address.hpp>
#include <actl/numeric/utility/hash_access.hpp>

namespace ac::detail {

// Special compressed_pair that mimics either T1 or T2 (defined by index I) for
// the operations required by set and hash set.
template <class T1, class T2, index I>
struct mimic_pair {
    T1 first;
    T2 second;

    struct is_mimic_pair;
    using value_type = std::conditional_t<I == 1, T1, T2>;
    using first_type = T1;
    using second_type = T2;

    static_assert(I == 1 || I == 2);

    template <class FirstT, class... SecondTs>
    explicit constexpr mimic_pair(FirstT&& first, SecondTs&&... xs)
        : first{std::forward<FirstT>(first)}
        , second{std::forward<SecondTs>(xs)...} {}

    constexpr decltype(auto) key() const {
        if constexpr (I == 1)
            return first;
        else
            return second;
    }

private:
    friend struct ac::hash_access;

    size_t hash() const {
        return hash_value(key());
    }
};

template <class T>
concept MimicPair = requires { typename T::is_mimic_pair; };

template <class T>
decltype(auto) get_key(T const& x) {
    if constexpr (MimicPair<T>) {
        return x.key();
    } else {
        return x;
    }
}

template <class T, class U>
    requires MimicPair<T> || MimicPair<U>
auto operator==(T const& lhs, U const& rhs) {
    return equal(get_key(lhs), get_key(rhs));
}

template <class T, class U>
    requires MimicPair<T> || MimicPair<U>
auto operator<(T const& lhs, U const& rhs) {
    return less(get_key(lhs), get_key(rhs));
}

template <class Map>
auto get_second(Map&& map) {
    using Pair = std::remove_reference_t<map_reference_t<Map>>;

    return composite_map{
        std::forward<Map>(map), static_member_map<&Pair::second>{}
    };
}

} // namespace ac::detail
