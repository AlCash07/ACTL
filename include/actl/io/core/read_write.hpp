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
#include <actl/range/span.hpp>
#include <actl/range/traits/concepts.hpp>
#include <actl/sequence/tuple/TupleConcept.hpp>

namespace ac::io::detail {

template<typename D, typename F, typename T>
bool write_pre_final(D& od, F& fmt, T const& t) {
    if constexpr (IO_Tuple<T>) {
        nested_scope_guard guard{fmt};
        return write_final(od, fmt, t);
    } else {
        return write_final(od, fmt, t);
    }
}

template<typename D, typename F, typename T>
bool read_pre_final(D& id, F& fmt, T&& t) {
    using U = std::remove_cvref_t<T>;
    if constexpr (IO_Tuple<U>) {
        nested_scope_guard guard{fmt};
        return read_final(id, fmt, t);
    } else {
        return read_final(id, fmt, t);
    }
}

// specialized for composed_format
template<typename D, typename FF, typename F, typename T>
struct format_resolver {
    static bool write(D& od, FF& full_fmt, F& fmt, T const& t) {
        return write_pre_final(od, full_fmt, apply_format_write(fmt, t));
    }

    static bool read(D& id, FF& full_fmt, F& fmt, T& t) {
        return read_pre_final(id, full_fmt, apply_format_read(fmt, t));
    }
};

template<typename D, typename FF, typename F, typename T, bool = Manipulator<T>>
struct arg_resolver : format_resolver<D, FF, F, T> {};

template<typename D, typename FF, typename F, typename T>
struct arg_resolver<D, FF, F, T, true> {
    static bool write(D&, FF&, F& fmt, T const& t) {
        manipulate(fmt, t);
        return true;
    }

    static bool read(D&, FF&, F& fmt, T& t) {
        manipulate(fmt, t);
        return true;
    }
};

template<typename D, typename FF, typename F, typename T>
bool write_impl(D& od, FF& full_fmt, F& fmt, T const& t) {
    return arg_resolver<D, FF, F, T>::write(od, full_fmt, fmt, t);
}

template<typename D, typename FF, typename F, typename T>
bool read_impl(D& id, FF& full_fmt, F& fmt, T&& t) {
    return arg_resolver<D, FF, F, std::remove_reference_t<T>>::read(
        id, full_fmt, fmt, t
    );
}

} // namespace ac::io::detail
