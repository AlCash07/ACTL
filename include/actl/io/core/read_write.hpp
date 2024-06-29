// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/io/core/batch.hpp>
#include <actl/io/core/manipulator.hpp>
#include <actl/io/core/serialization_access.hpp>
#include <actl/io/format/apply_format.hpp>
#include <actl/meta/tuple.hpp>
#include <actl/range/span.hpp>
#include <actl/range/traits/concepts.hpp>

namespace ac::io::detail {

template <class D, class F, class T>
bool write_pre_final(D& od, F& fmt, T const& x) {
    if constexpr (IO_Tuple<T>) {
        nested_scope_guard g{fmt};
        return write_final(od, fmt, x);
    } else {
        return write_final(od, fmt, x);
    }
}

template <class D, class F, class T>
bool read_pre_final(D& id, F& fmt, T&& x) {
    using U = std::remove_cvref_t<T>;
    if constexpr (IO_Tuple<U>) {
        nested_scope_guard g{fmt};
        return read_final(id, fmt, x);
    } else {
        return read_final(id, fmt, x);
    }
}

// specialized for composed_format
template <class D, class FF, class F, class T>
struct format_resolver {
    static bool write(D& od, FF& full_fmt, F& fmt, T const& x) {
        return write_pre_final(od, full_fmt, apply_format_write(fmt, x));
    }

    static bool read(D& id, FF& full_fmt, F& fmt, T& x) {
        return read_pre_final(id, full_fmt, apply_format_read(fmt, x));
    }
};

template <class D, class FF, class F, class T, bool = Manipulator<T>>
struct arg_resolver : format_resolver<D, FF, F, T> {};

template <class D, class FF, class F, class T>
struct arg_resolver<D, FF, F, T, true> {
    static bool write(D&, FF&, F& fmt, T const& x) {
        manipulate(fmt, x);
        return true;
    }

    static bool read(D&, FF&, F& fmt, T& x) {
        manipulate(fmt, x);
        return true;
    }
};

template <class D, class FF, class F, class T>
bool write_impl(D& od, FF& full_fmt, F& fmt, T const& x) {
    return arg_resolver<D, FF, F, T>::write(od, full_fmt, fmt, x);
}

template <class D, class FF, class F, class T>
bool read_impl(D& id, FF& full_fmt, F& fmt, T&& x) {
    return arg_resolver<D, FF, F, std::remove_reference_t<T>>::read(
        id, full_fmt, fmt, x
    );
}

} // namespace ac::io::detail
