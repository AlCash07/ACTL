// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/io/core/read_write.hpp>
#include <actl/std/tuple.hpp>

namespace ac::io {

template <class... Ts>
struct batch : std::tuple<Ts...> {
    using std::tuple<Ts...>::tuple;
};

template <class... Ts>
batch(Ts&&...) -> batch<Ts...>;

template <class D, class F, class T, size_t... Is>
index write_final_batch(D& od, F& fmt, const T& x, std::index_sequence<Is...>) {
    return (... + write_final(od, fmt, std::get<Is>(x)));
}

template <class D, class F, class... Ts>
index write_pre_final(D& od, F& fmt, const batch<Ts...>& x) {
    return write_final_batch(od, fmt, x, std::make_index_sequence<sizeof...(Ts)>{});
}

template <class D, class F, class T, size_t... Is>
bool read_final_batch(D& id, F& fmt, T& x, std::index_sequence<Is...>) {
    return (... && read_final(id, fmt, std::get<Is>(x)));
}

template <class D, class F, class... Ts>
bool read_pre_final(D& id, F& fmt, batch<Ts...>& x) {
    return read_final_batch(id, fmt, x, std::make_index_sequence<sizeof...(Ts)>{});
}

namespace detail {

template <class... Ts>
struct batch_resolver;

template <class D, class FF, class F, class Batch, size_t... Is>
struct batch_resolver<D, FF, F, Batch, std::index_sequence<Is...>> {
    static index write(D& od, FF& full_fmt, F& fmt, const Batch& x) {
        return (... + write_impl(od, full_fmt, fmt, std::get<Is>(x)));
    }

    static bool read(D& id, FF& full_fmt, F& fmt, Batch& x) {
        return (... && read_impl(id, full_fmt, fmt, std::get<Is>(x)));
    }
};

template <class D, class FF, class F, class... Ts>
struct arg_resolver<D, FF, F, batch<Ts...>, false>
    : batch_resolver<D, FF, F, batch<Ts...>, std::make_index_sequence<sizeof...(Ts)>> {};

}  // namespace detail

}  // namespace ac::io
