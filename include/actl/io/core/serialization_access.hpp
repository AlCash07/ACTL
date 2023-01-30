// Copyright 2019 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/functional/deduce_noexcept.hpp>
#include <actl/meta/type_traits.hpp>

namespace ac::io {

struct serialization_access
{
    // All the checks are inside serialization_access because they check for
    // private functionality.
    template <class T>
    static constexpr bool is_io_tuple_v = requires { typename T::is_io_tuple; };

    template <class T, class... Ts>
    static bool write_final(T const& x, Ts&... args)
        AC_DEDUCE_NOEXCEPT_REQUIRES_AND_RETURN(x.write_final(args...))

    template <class T, class... Ts>
    static bool read(T& x, Ts&&... args)
        AC_DEDUCE_NOEXCEPT_REQUIRES_AND_RETURN(x.read_final(args...))
};

template <class T>
concept IO_Tuple = serialization_access::is_io_tuple_v<T>;

template <class T>
bool write_final(Device auto& od, Format auto& fmt, T const& x)
    AC_DEDUCE_NOEXCEPT_REQUIRES_AND_RETURN(
        serialization_access::write_final(x, od, fmt))

template <class T>
bool read_final(Device auto& id, Format auto& fmt, T& x)
    AC_DEDUCE_NOEXCEPT_REQUIRES_AND_RETURN(
        serialization_access::read(x, id, fmt))

} // namespace ac::io
