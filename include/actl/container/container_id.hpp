// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/assert.hpp>
#include <actl/container/functions.hpp>
#include <actl/iterator/integer_iterator.hpp>
#include <actl/iterator/interface/iterator_adaptor.hpp>
#include <actl/numeric/bit/bit_cast.hpp>
#include <actl/numeric/utility/hash_access.hpp>
#include <actl/range/iterator_range.hpp>
#include <actl/range/traits/associated_types.hpp>
#include <cstdint>

namespace ac {

template <class C, bool = Container<C>, bool = RandomAccessRange<C>>
struct container_id_traits;

template <class Iter>
class iterator_id : public iterator_adaptor<iterator_id<Iter>, Iter>
{
    using base_t = iterator_adaptor<iterator_id<Iter>, Iter>;

public:
    using value_type = iterator_id<Iter>;

    explicit iterator_id(Iter iter = {}) noexcept(
        std::is_nothrow_copy_constructible_v<Iter>)
        : base_t{iter}
    {}

    explicit iterator_id(void* raw) : base_t{bit_cast<Iter>(raw)}
    {
        // TODO: implement more general logic in case this condition fails.
        static_assert(sizeof(Iter) == sizeof(void*));
    }

    friend std::uintptr_t get_id_key(iterator_id id)
    {
        return reinterpret_cast<std::uintptr_t>(std::addressof(*id.base()));
    }

    bool operator<(iterator_id rhs) const
    {
        return get_id_key(*this) < get_id_key(rhs);
    }

    iterator_id operator*() const
        noexcept(std::is_nothrow_copy_constructible_v<Iter>)
    {
        return *this;
    }

private:
    friend struct ac::hash_access;

    size_t hash() const
    {
        return hash_value(get_id_key(*this));
    }
};

template <class C>
struct container_id_traits<C, true, false>
{
    using id = iterator_id<range_iterator_t<C const>>;
    using iterator = id;
};

template <class C>
struct container_id_traits<C, true, true>
{
    using id = int;
    using iterator = integer_iterator<id>;
};

inline constexpr int get_id_key(int id)
{
    return id;
}

inline constexpr int id_to_raw(int id)
{
    return id;
}

template <class Iter>
void* id_to_raw(iterator_id<Iter> id)
{
    return bit_cast<void*>(id);
}

/// Container Id is int for random access containers and wrapped const_iterator
/// otherwise. Such Id isn't invalidated by emplace operation and can be used as
/// map or hash map key.
template <class C>
using container_id = typename container_id_traits<C>::id;

template <class C>
using container_id_iterator = typename container_id_traits<C>::iterator;

// This key can be used for id comparison.
template <class Id>
using id_key_t = decltype(get_id_key(std::declval<Id>()));

template <class C>
container_id<C> id_begin(C const& cont)
{
    if constexpr (RandomAccessRange<C>)
        return 0;
    else
        return container_id<C>{cont.begin()};
}

template <class C>
container_id<C> id_end(C const& cont)
{
    if constexpr (RandomAccessRange<C>)
        return static_cast<container_id<C>>(ranges::size(cont));
    else
        return container_id<C>{cont.end()};
}

// Returns an invalid Id that is fixed for the given container.
template <class C>
container_id<C> id_null(C const& cont)
{
    if constexpr (RandomAccessRange<C>)
        return -1;
    else
        return id_end(cont);
}

template <class C>
auto id_range(C const& cont)
{
    using iterator = container_id_iterator<C>;
    return make_range(iterator{id_begin(cont)}, iterator{id_end(cont)});
}

template <class C>
container_id<C> iterator_to_id(C const& cont, range_iterator_t<C const> iter)
{
    if constexpr (RandomAccessRange<C>)
        return static_cast<container_id<C>>(iter - ranges::begin(cont));
    else
        return container_id<C>{iter};
}

template <class C>
range_iterator_t<C const> id_to_iterator(C const& cont, container_id<C> id)
{
    if constexpr (RandomAccessRange<C>)
    {
        AC_ASSERT(0 <= id && id <= id_end(cont));
        return ranges::begin(cont) + id;
    }
    else
    {
        return id.base();
    }
}

template <class C>
void id_check(C& cont, container_id<C> id)
{
    if constexpr (RandomAccessRange<C>)
    {
        AC_ASSERT(0 <= id && id < id_end(cont));
    }
    else
    {
        AC_ASSERT(id != id_end(cont));
    }
}

template <class C>
reference_t<C> id_at(C& cont, container_id<C> id)
{
    id_check(cont, id);
    if constexpr (RandomAccessRange<C>)
        return cont[static_cast<range_size_t<C>>(id)];
    else
        // const_cast is required because id contains a const_iterator.
        // TODO: this cast allows modification of set key, which may lead to
        // bugs.
        return const_cast<reference_t<C>>(*id.base());
}

template <class C>
reference_t<C const> id_at(C const& cont, container_id<C> id)
{
    id_check(cont, id);
    if constexpr (RandomAccessRange<C>)
        return cont[static_cast<range_size_t<C>>(id)];
    else
        return *id.base();
}

/* Generic container functions with id */

template <class C, class... Ts>
std::pair<container_id<C>, bool> id_emplace(C& cont, Ts&&... args)
{
    auto res = emplace(cont, std::forward<Ts>(args)...);
    return {iterator_to_id(cont, res.first), res.second};
}

template <class C>
void id_erase(C& cont, container_id<C> id)
{
    cont.erase(id_to_iterator(cont, id));
}

template <class C, class T>
container_id<C> id_find(C const& cont, T const& value)
{
    return iterator_to_id(cont, find(cont, value));
}

} // namespace ac
