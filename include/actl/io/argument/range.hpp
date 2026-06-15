// Copyright 2019 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/container/functions.hpp>
#include <actl/io/argument/byte_span.hpp>
#include <actl/io/argument/size.hpp>
#include <actl/io/core/batch.hpp>
#include <actl/meta/static_size.hpp>
#include <actl/range/traits/associated_types.hpp>
#include <actl/range/traits/associative_range.hpp>
#include <actl/range/traits/concepts.hpp>
#include <actl/range/traits/properties.hpp>

namespace ac::io {

template<typename Pair>
decltype(auto) key_representation(Pair& pair) {
    if constexpr (std::is_const_v<Pair>)
        return pair.first;
    else
        // const_cast is used to read std::map<Key, Value>::value_type
        // which is std::pair<Key const, Value>.
        return const_cast<typename Pair::first_type&>(pair.first);
}

template<typename Range, typename T>
decltype(auto) element_representation(T& pair) {
    if constexpr (PairAssociativeRange<Range>)
        return batch{key_representation(pair), colon{}, pair.second};
    else
        return pair;
}

template<Range R>
bool write_final(Device auto& od, Format auto& fmt, R const& range) {
    nested_scope_guard guard{fmt};
    if constexpr (Container<R> && static_size_v<R> == dynamic_size)
        if (!write(od, fmt, size{range.size()}))
            return false;
    for (auto const& element : range)
        if (!write(od, fmt, element_representation<R>(element)))
            return false;
    return true;
}

bool read_range(Device auto& id, Format auto& fmt, Range auto& range) {
    for (auto& element : range) {
        if (!read(id, fmt, element))
            return false;
    }
    return true;
}

template<typename C>
bool read_container(Device auto& id, Format auto& fmt, C& cont) {
    decltype(cont.size()) size{};
    if (!read(id, fmt, io::size{size}))
        return false;
    if constexpr (!RandomAccessRange<C>) {
        for (; size > 0; --size) {
            range_value_t<C> element;
            if (!read(id, fmt, element_representation<C>(element)))
                return false;
            emplace(cont, std::move(element));
        }
        return true;
    } else {
        cont.resize(size);
        return read_range(id, fmt, cont);
    }
}

template<Range R>
    requires(!std::is_const_v<range_value_t<R>>)
bool read_final(Device auto& id, Format auto& fmt, R& range) {
    nested_scope_guard guard{fmt};
    if constexpr (Container<R> && static_size_v<R> == dynamic_size)
        return read_container(id, fmt, range);
    else
        return read_range(id, fmt, range);
}

} // namespace ac::io
