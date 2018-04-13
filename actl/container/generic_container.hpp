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
#include <actl/range/range.hpp>
#include <actl/type/none.hpp>
#include <actl/type_traits/container_traits.hpp>
#include <algorithm>
#include <cstdint>

namespace ac {

template <class C>
class iterator_id;

template <class C>
class generic_container_base {
public:
    using size_type              = int;
    using difference_type        = int;
    using value_type             = typename C::value_type;
    using allocator_type         = typename C::allocator_type;
    using reference              = typename C::reference;
    using const_reference        = typename C::const_reference;
    using pointer                = typename C::pointer;
    using const_pointer          = typename C::const_pointer;
    using iterator               = typename C::iterator;
    using const_iterator         = typename C::const_iterator;
    using reverse_iterator       = typename C::reverse_iterator;
    using const_reverse_iterator = typename C::const_reverse_iterator;

    bool empty() const { return data_.empty(); }

    size_type size() const { return static_cast<size_type>(data_.size()); }

    void clear() { data_.clear(); }

    iterator begin() { return data_.begin(); }
    iterator end()   { return data_.end(); }

    const_iterator begin() const { return data_.begin(); }
    const_iterator end()   const { return data_.end(); }

    reverse_iterator rbegin() { return reverse_iterator(end()); }
    reverse_iterator rend()   { return reverse_iterator(begin()); }

    const_reverse_iterator rbegin() const { return const_reverse_iterator(end()); }
    const_reverse_iterator rend()   const { return const_reverse_iterator(begin()); }

    const_iterator cbegin() const { return begin(); }
    const_iterator cend()   const { return end(); }

    const_reverse_iterator crbegin() const { return rbegin(); }
    const_reverse_iterator crend()   const { return rend(); }

protected:
    template <class... Ts>
    generic_container_base(Ts&&... args) : data_(std::forward<Ts>(args)...) {}

    C data_;
};

template <class C, class T = typename C::value_type, bool RA = is_random_access_v<C>>
class generic_container : public generic_container_base<C> {
    using generic_container_base<C>::data_;

public:
    using id          = iterator_id<C>;
    using id_iterator = typename id::iterator;

    explicit generic_container() = default;

    explicit generic_container(int n) {
        for (int i = 0; i < n; ++i) emplace();
    }

    range<id_iterator> id_range() const {
        return {id_iterator(data_.begin()), id_iterator(data_.end())};
    }

    // Returns an invalid Id that is fixed for the given container.
    id null_id() const { return data_.end(); }

    typename C::reference operator[](id it) {
        // const_cast is required because id is a const_iterator.
        return const_cast<typename C::reference>(*it);
    }

    typename C::const_reference operator[](id it) const { return *it; }

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

    void erase(id it) { data_.erase(it); }

    void swap(generic_container& other) { data_.swap(other.data_); }
};

template <class C, class T>
class generic_container<C, T, true> : public generic_container_base<C> {
    using generic_container_base<C>::data_;

public:
    using id          = int;
    using id_iterator = integer_iterator<id>;

    explicit generic_container() = default;

    explicit generic_container(int n) : generic_container_base<C>(n) {}

    range<id_iterator> id_range() const { return {id_iterator(0), id_iterator(this->size())}; }

    id null_id() const { return -1; }

    typename C::reference operator[](id i) { return data_[i]; }

    typename C::const_reference operator[](id i) const { return data_[i]; }

    template <class U>
    id find(const U& value) {
        auto it = std::find(data_.begin(), data_.end(), value);
        return it == data_.end() ? null_id() : static_cast<id>(it - data_.begin());
    }

    template <class... Ts>
    std::pair<id, bool> emplace(Ts&&... args) {
        data_.emplace_back(std::forward<Ts>(args)...);
        return {this->size() - 1, true};
    }

    void erase(id i) {
        ACTL_ASSERT(0 <= i && i < size());
        data_.erase(data_.begin() + i);
    }

    void resize(int n) { data_.resize(n); }

    void swap(generic_container& other) { data_.swap(other.data_); }
};

template <class C>
class generic_container<C, none, true> {
public:
    using id          = int;
    using id_iterator = integer_iterator<id>;

    explicit generic_container(int n = 0) : n_(n) {}

    range<id_iterator> id_range() const { return {id_iterator(0), id_iterator(size())}; }

    id null_id() const { return -1; }

    int size() const { return n_; }

    bool empty() const { return size() == 0; }

    std::pair<id, bool> emplace() { return {n_++, true}; }

    void erase(id i) {
        ACTL_ASSERT(0 <= i && i < n_);
        --n_;
    }

    void clear() { n_ = 0; }

    void resize(int n) { n_ = n; }

    void swap(generic_container& other) { std::swap(n_, other.n_); }

protected:
    int n_;
};

template <class C>
using container_id_t = typename generic_container<C>::id;

// Id is implicitly convertible to this key type, and it can be used for comparison.
template <class Id> struct id_key      { using type = std::uintptr_t; };
template <>         struct id_key<int> { using type = int; };

template <class Id>
using id_key_t = typename id_key<Id>::type;

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
        Id dereference() const { return this->base(); }

        friend struct ac::iterator_core_access;
    };

    constexpr operator std::uintptr_t() const {
        return reinterpret_cast<std::uintptr_t>(std::addressof(*this));
    }

    friend class generic_container<C>;
};

// This wrapper can be used to avoid Id types collision.
template <class Id>
class wrap_id {
    Id id_;

public:
    constexpr wrap_id(Id id) : id_(id) {}

    constexpr operator Id() const { return id_; }
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
    Id dereference() const { return *this->base(); }

    friend struct ac::iterator_core_access;
};

}  // namespace ac

namespace std {

template <class C>
struct hash<ac::iterator_id<C>> : hash<std::uintptr_t> {};

template <class Id>
struct hash<ac::wrap_id<Id>> : hash<Id> {};

}  // namespace std
