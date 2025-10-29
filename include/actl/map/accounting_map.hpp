// Copyright 2017 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/map/traits.hpp>
#include <actl/memory/AC_NO_UNIQUE_ADDRESS.hpp>
#include <actl/range/traits/associative_range.hpp>
#include <actl/range/traits/properties.hpp>
#include <actl/std/vector.hpp>

namespace ac {

/// Property map into integer domain that assigns next non-negative integer to
/// every key not encountered before. Can be invertible with overhead of
/// additional vector of pointers.
template<typename AssociativeContainer, bool Invertible = false>
class accounting_map {
    using AC = AssociativeContainer;
    using K = typename AC::key_type;
    using Key = K const&;
    using V = typename AC::mapped_type;

public:
    static_assert(UniqueRange<AC> && PairAssociativeRange<AC>);
    static_assert(std::is_integral_v<V>, "value type must be integral");

    struct is_accounting_map;

    template<bool Const>
    using traits = map_traits_base<
        Key,
        V const&,
        V,
        !Const,
        false,
        Invertible,
        true,
        AC const&>;

    V const& get(Key key) {
        auto pair = m_range.insert({key, static_cast<V>(m_range.size())});
        if constexpr (Invertible) {
            if (pair.second)
                m_inverse.push_back(&pair.first->first);
        }
        return pair.first->second;
    }

    Key invert(V const& value) const {
        return *m_inverse[static_cast<size_t>(value)];
    }

    AC const& map_range() const {
        return m_range;
    }

private:
    AC_NO_UNIQUE_ADDRESS AC m_range;
    AC_NO_UNIQUE_ADDRESS
    std::conditional_t<Invertible, std::vector<K const*>, none> m_inverse;
};

template<typename AM>
    requires requires { typename AM::is_accounting_map; }
struct map_traits<AM> : AM::template traits<std::is_const_v<AM>> {};

} // namespace ac
