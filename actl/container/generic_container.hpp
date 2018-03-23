/***************************************************************************************************
 * Wrapper for standard containers with generic interface.
 ***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/assert.hpp>
#include <actl/iterator/integer_iterator.hpp>
#include <actl/iterator/iterator_adaptor.hpp>
#include <actl/property_map/property_map.hpp>
#include <actl/range/range.hpp>
#include <actl/type/none.hpp>
#include <actl/type_traits/container_traits.hpp>
#include <algorithm>
#include <cstdint>

namespace ac {

namespace detail {

// Iterator that dereferences into container ID that is also an iterator.
template <class ID>
class id_iterator : public iterator_adaptor<id_iterator<ID>, ID, use_default, ID, ID, ID*> {
public:
    explicit id_iterator(ID id)
        : iterator_adaptor<id_iterator<ID>, ID, use_default, ID, ID, ID*>(id) {}

private:
    friend struct ac::iterator_core_access;

    ID dereference() const { return this->base(); }
};

template <class C, class ID>
using generic_container_base =
    container_property_map<false, C, ID, typename C::value_type, typename C::reference,
                           typename C::const_reference>;

}  // namespace detail

template <class C, class T = typename C::value_type, bool RA = is_random_access<C>::value>
class generic_container : public detail::generic_container_base<C, typename C::const_iterator> {
    using detail::generic_container_base<C, typename C::const_iterator>::data_;

public:
    using id          = typename C::const_iterator;
    using id_iterator = detail::id_iterator<id>;

    typename C::reference       operator[](id id) { return const_cast<typename C::reference>(*id); }
    typename C::const_reference operator[](id id) const { return *id; }

    auto id_range() const {
        return make_range(id_iterator(data_.begin()), id_iterator(data_.end()));
    }

    // Returns an invalid ID that is fixed for the given container.
    id void_id() const { return data_.end(); }

    int size() const { return static_cast<int>(data_.size()); }

    template <class U>
    id find(const U& value) {
        if constexpr (is_associative<C>::value) {
            return data_.find(value);
        } else {
            return std::find(data_.begin(), data_.end(), value);
        }
    }

    template <class... Ts>
    std::pair<id, bool> emplace(Ts&&... args) {
        if constexpr (is_associative<C>::value) {
            auto res = data_.emplace(std::forward<Ts>(args)...);
            if constexpr (is_unique_associative<C>::value) {
                return res;
            } else {
                return {res, true};
            }
        } else {
            return {data_.emplace(data_.end(), std::forward<Ts>(args)...), true};
        }
    }

    void erase(id id) { data_.erase(id); }
};

template <class C, class T>
class generic_container<C, T, true> : public detail::generic_container_base<C, int> {
    using detail::generic_container_base<C, int>::data_;

public:
    using id          = int;
    using id_iterator = integer_iterator<id>;

    typename C::reference       operator[](id id) { return data_[id]; }
    typename C::const_reference operator[](id id) const { return data_[id]; }

    auto id_range() const { return make_range(id_iterator(0), id_iterator(size())); }

    id void_id() const { return -1; }

    int size() const { return static_cast<int>(data_.size()); }

    template <class U>
    id find(const U& value) {
        auto it = std::find(data_.begin(), data_.end(), value);
        return it == data_.end() ? void_id() : static_cast<id>(it - data_.begin());
    }

    template <class... Ts>
    std::pair<id, bool> emplace(Ts&&... args) {
        data_.emplace_back(std::forward<Ts>(args)...);
        return {size() - 1, true};
    }

    void erase(id id) {
        ACTL_ASSERT(0 <= id && id < size());
        data_.erase(data_.begin() + id);
    }
};

template <class C>
class generic_container<C, none, true> {
public:
    using id          = int;
    using id_iterator = integer_iterator<id>;

    auto id_range() const { return make_range(id_iterator(0), id_iterator(size())); }

    id void_id() const { return -1; }

    int size() const { return n_; }

    std::pair<id, bool> emplace() { return {n_++, true}; }

    void erase(id id) {
        ACTL_ASSERT(0 <= id && id < n_);
        --n_;
    }

protected:
    int n_;
};

/* Container ID: int for random access containers, iterator otherwise. */

template <class C>
using container_id_t = typename generic_container<C>::id;

// ID key that can be used for comparison and hashing.
constexpr inline auto get_id_key(int value) { return value; }

template <class ID>
constexpr inline auto get_id_key(ID id) {
    return reinterpret_cast<std::uintptr_t>(std::addressof(*id));
}

}  // namespace ac
