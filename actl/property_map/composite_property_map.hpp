/***************************************************************************************************
 * composite_property_map combines multiple property pms, applying them in the given order.
 * Supports inversion, traversal and put if at all possible.
 ***************************************************************************************************
 * Copyright 2017 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/iterator/iterator_adaptor.hpp>
#include <actl/property_map/property_map.hpp>
#include <actl/type/compressed_pair.hpp>
#include <type_traits>
#include <utility>

namespace ac {

namespace detail {

// Base class containing the property maps and implementing common methods.
template <class PM1, class PM2>
class composite_pm_base : public property_map_base, protected compressed_pair<PM1, PM2> {
    using base_t = compressed_pair<PM1, PM2>;

public:
    static_assert(std::is_convertible_v<typename property_traits<PM1>::reference,
                                        typename property_traits<PM2>::key_type>,
                  "incompatible property maps");

    using key_type   = typename property_traits<PM1>::key_type;
    using value_type = typename property_traits<PM2>::value_type;
    using reference  = typename property_traits<PM2>::reference;

    using base_t::base_t;

    friend constexpr reference get(const composite_pm_base& pm, key_type key) {
        return get(pm.second(), get(pm.first(), key));
    }
};

// Class that adds put function if possible.
template <class PM1, class PM2, bool, bool, bool>
class composite_pm_put : public composite_pm_base<PM1, PM2> {
public:
    static constexpr bool writable = false;

    using composite_pm_base<PM1, PM2>::composite_pm_base;
};

template <class PM1, class PM2, bool W1, bool Inv2>
class composite_pm_put<PM1, PM2, W1, true, Inv2> : public composite_pm_base<PM1, PM2> {
    using base_t = composite_pm_base<PM1, PM2>;

public:
    static constexpr bool writable = true;

    using base_t::base_t;

    friend constexpr void put(const composite_pm_put& pm, typename base_t::key_type key,
                              typename base_t::value_type value) {
        put(pm.second(), get(pm.first(), key), value);
    }
};

template <class PM1, class PM2>
class composite_pm_put<PM1, PM2, true, false, true> : public composite_pm_base<PM1, PM2> {
    using base_t = composite_pm_base<PM1, PM2>;

public:
    static constexpr bool writable = true;

    using base_t::base_t;

    friend constexpr void put(const composite_pm_put& pm, typename base_t::key_type key,
                              typename base_t::value_type value) {
        put(pm.first(), key, pm.second().invert(value));
    }
};

template <class PM1, class PM2>
using composite_pm_put_t =
    composite_pm_put<PM1, PM2, property_traits<PM1>::writable, property_traits<PM2>::writable,
                     property_traits<PM2>::invertible>;

// Class that adds invert method if possible.
template <class PM1, class PM2, bool>
class composite_pm_invert : public composite_pm_put_t<PM1, PM2> {
public:
    static constexpr bool invertible = false;

    using composite_pm_put_t<PM1, PM2>::composite_pm_put_t;
};

template <class PM1, class PM2>
class composite_pm_invert<PM1, PM2, true> : public composite_pm_put_t<PM1, PM2> {
    using base_t = composite_pm_put_t<PM1, PM2>;

public:
    static constexpr bool invertible = true;

    using base_t::base_t;

    constexpr typename base_t::key_type invert(typename base_t::value_type value) const {
        return this->first().invert(this->second().invert(value));
    }
};

template <class PM1, class PM2>
using composite_pm_invert_t =
    composite_pm_invert<PM1, PM2,
                        property_traits<PM1>::invertible && property_traits<PM2>::invertible>;

// Class that adds begin/end methods if possible.
template <class PM1, class PM2, bool, bool, bool>
class composite_pm_iterate : public composite_pm_invert_t<PM1, PM2> {
public:
    static constexpr bool iterable = false;

    using composite_pm_invert_t<PM1, PM2>::composite_pm_invert_t;
};

template <class PM1, class PM2, bool It2, bool Inv2>
class composite_pm_iterate<PM1, PM2, true, It2, Inv2> : public composite_pm_invert_t<PM1, PM2> {
    using base_t = composite_pm_invert_t<PM1, PM2>;
    using It     = typename PM1::iterator;
    using Pair   = std::pair<typename base_t::key_type, typename base_t::reference>;

public:
    class iterator
        : public iterator_adaptor<iterator, It, use_default, Pair, Pair, Pair*, use_default> {
        iterator(const It& it, const PM2& pm)
            : iterator_adaptor<iterator, It, use_default, Pair, Pair, Pair*, use_default>(it),
              pm_{pm} {}

        Pair dereference() const { return {this->base()->first, get(pm_, this->base()->second)}; }

        const PM2& pm_;

        friend class composite_pm_iterate;
        friend struct ac::iterator_core_access;
    };

    static constexpr bool iterable = true;

    using base_t::base_t;

    iterator begin() const { return {this->first().begin(), this->second()}; }
    iterator end()   const { return {this->first().end(),   this->second()}; }
};

template <class PM1, class PM2>
class composite_pm_iterate<PM1, PM2, false, true, true> : public composite_pm_invert_t<PM1, PM2> {
    using base_t = composite_pm_invert_t<PM1, PM2>;
    using It     = typename PM2::iterator;
    using Pair   = std::pair<typename base_t::key_type, typename base_t::reference>;

public:
    class iterator
        : public iterator_adaptor<iterator, It, use_default, Pair, Pair, Pair*, use_default> {
        iterator(const It& it, const PM1& pm)
            : iterator_adaptor<iterator, It, use_default, Pair, Pair, Pair*, use_default>(it),
              pm_{pm} {}

        Pair dereference() const { return {pm_.invert(this->base()->first), this->base()->second}; }

        const PM1& pm_;

        friend class composite_pm_iterate;
        friend struct ac::iterator_core_access;
    };

    static constexpr bool iterable = true;

    using base_t::base_t;

    iterator begin() const { return {this->second().begin(), this->first()}; }
    iterator end()   const { return {this->second().end(),   this->first()}; }
};

template <class PM1, class PM2>
using composite_pm_t =
    composite_pm_iterate<PM1, PM2, property_traits<PM1>::iterable, property_traits<PM2>::iterable,
                         property_traits<PM1>::invertible>;

}  // namespace detail

template <class PM1, class PM2, class... PMs>
class composite_property_map
    : public composite_property_map<composite_property_map<PM1, PM2>, PMs...> {};

template <class PM1, class PM2>
class composite_property_map<PM1, PM2> : public detail::composite_pm_t<PM1, PM2> {
    using base_t = detail::composite_pm_t<PM1, PM2>;

public:
    using base_t::base_t;
};

template <class PM>
inline PM make_composite_property_map(PM&& pm) {
    return pm;
}

template <class PM1, class PM2, class... PMs>
inline auto make_composite_property_map(PM1&& pm1, PM2&& pm2, PMs&&... pms) {
    return make_composite_property_map(
        composite_property_map<PM1, PM2>(std::forward<PM1>(pm1), std::forward<PM2>(pm2)),
        std::forward<PMs>(pms)...);
}

}  // namespace ac
