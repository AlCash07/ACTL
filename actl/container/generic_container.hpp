/***************************************************************************************************
 * generic_container is a wrapper for standard containers with generic interface.
 * Instead of iterators, it operates with container Id, which is int for random access containers
 * and wrapped const_iterator otherwise.
 * Such Id isn't invalidated by push_back operation and can be used as map or hash map key.
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

template <class C>
class iterator_id;

template <class C, bool RA = is_random_access_v<C>>
struct container_id {
    using type     = iterator_id<C>;
    using iterator = typename type::iterator;
};

template <class C>
struct container_id<C, true> {
    using type     = int;
    using iterator = integer_iterator<type>;
};

template <class C>
using container_id_t = typename container_id<C>::type;

template <class Id> struct id_key      { using type = std::uintptr_t; };
template <>         struct id_key<int> { using type = int; };

template <class Id>
using id_key_t = typename id_key<Id>::type;

template <class C>
using generic_container_base = container_property_map<C, container_id_t<C>, typename C::value_type,
                                                      typename C::reference, false>;

// This class is used to hide container operations when only property map interface is requested.
template <class C>
class generic_container_property_map : public generic_container_base<C> {
    using base_t = generic_container_base<C>;

public:
    using id = container_id_t<C>;

    using base_t::base_t;

    // const_cast is required because id is a const_iterator.
    typename C::reference operator[](id id) {
        if constexpr (is_random_access_v<C>) {
            return this->data_[id];
        } else {
            return const_cast<typename C::reference>(*id);
        }
    }

    typename C::const_reference operator[](id id) const {
        if constexpr (is_random_access_v<C>) {
            return this->data_[id];
        } else {
            return *id;
        }
    }
};

template <class C, class T = typename C::value_type, bool RA = is_random_access_v<C>>
class generic_container : public generic_container_property_map<C> {
public:
    using id           = container_id_t<C>;
    using id_iterator  = typename container_id<C>::iterator;
    using property_map = generic_container_property_map<C>;
    using property_map::data_;

    explicit generic_container() = default;

    explicit generic_container(int n) {
        for (int i = 0; i < n; ++i) emplace();
    }

    range<id_iterator> id_range() const {
        return {id_iterator(data_.begin()), id_iterator(data_.end())};
    }

    // Returns an invalid Id that is fixed for the given container.
    id null_id() const { return data_.end(); }

    int size() const { return static_cast<int>(data_.size()); }

    template <class U>
    id find(const U& value) const {
        if constexpr (is_associative_v<C>) {
            return data_.find(value);
        } else {
            return std::find(data_.begin(), data_.end(), value);
        }
    }

    template <class... Ts>
    std::pair<id, bool> emplace(Ts&&... args) {
        if constexpr (is_associative_v<C>) {
            auto res = data_.emplace(std::forward<Ts>(args)...);
            if constexpr (is_unique_associative<C>::value) {
                return {res.first, res.second};
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
class generic_container<C, T, true> : public generic_container_property_map<C> {
public:
    using id           = container_id_t<C>;
    using id_iterator  = typename container_id<C>::iterator;
    using property_map = generic_container_property_map<C>;
    using property_map::data_;

    explicit generic_container() = default;

    explicit generic_container(int n) : property_map(n) {}

    range<id_iterator> id_range() const { return {id_iterator(0), id_iterator(size())}; }

    id null_id() const { return -1; }

    int size() const { return static_cast<int>(data_.size()); }

    template <class U>
    id find(const U& value) {
        auto it = std::find(data_.begin(), data_.end(), value);
        return it == data_.end() ? null_id() : static_cast<id>(it - data_.begin());
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

    void resize(int n) { data_.resize(n); }
};

template <class C>
class generic_container<C, none, true> {
public:
    using id          = container_id_t<C>;
    using id_iterator = typename container_id<C>::iterator;

    explicit generic_container(int n = 0) : n_(n) {}

    range<id_iterator> id_range() const { return {id_iterator(0), id_iterator(size())}; }

    id null_id() const { return -1; }

    int size() const { return n_; }

    std::pair<id, bool> emplace() { return {n_++, true}; }

    void erase(id id) {
        ACTL_ASSERT(0 <= id && id < n_);
        --n_;
    }

    void clear() { n_ = 0; }

    void resize(int n) { n_ = n; }

    void swap(generic_container& other) { std::swap(n_, other.n_); }

protected:
    int n_;
};

template <class C>
class iterator_id : public C::const_iterator {
    using Id = iterator_id;
    using It = typename C::const_iterator;

    constexpr iterator_id(It it) : It(it) {}

public:
    class iterator : public iterator_adaptor<iterator, It, use_default, Id, Id, Id*> {
    public:
        explicit iterator(It it) : iterator_adaptor<iterator, It, use_default, Id, Id, Id*>(it) {}

    private:
        friend struct ac::iterator_core_access;

        Id dereference() const { return this->base(); }
    };

    constexpr operator std::uintptr_t() const {
        return reinterpret_cast<std::uintptr_t>(std::addressof(*this));
    }

    friend class generic_container_property_map<C>;
    friend class generic_container<C>;
};

// This wrapper can be used to avoid Id types collision.
template <class Id>
class wrap_id {
    Id id;

public:
    explicit constexpr wrap_id(Id id) : id(id) {}

    constexpr operator Id() const { return id; }
};

template <class Id>
struct id_key<wrap_id<Id>> : id_key<Id> {};

template <class It, class Id = wrap_id<typename std::iterator_traits<It>::value_type>>
class wrap_id_iterator
    : public iterator_adaptor<wrap_id_iterator<It, Id>, It, use_default, Id, Id, Id*> {
public:
    explicit wrap_id_iterator(It it)
        : iterator_adaptor<wrap_id_iterator<It, Id>, It, use_default, Id, Id, Id*>(it) {}

private:
    friend struct ac::iterator_core_access;

    Id dereference() const { return *this->base(); }
};

}  // namespace ac

namespace std {

template <class C>
struct hash<ac::iterator_id<C>> : hash<std::uintptr_t> {};

template <class Id>
struct hash<ac::wrap_id<Id>> : hash<Id> {};

}  // namespace std
