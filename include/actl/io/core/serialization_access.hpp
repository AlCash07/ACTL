// Copyright 2019 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/functional/noexcept/AC_DEDUCE_NOEXCEPT_REQUIRES_AND_RETURN.hpp>

namespace ac::io {

struct serialization_access {
    // All the checks are inside serialization_access because they check for
    // private functionality.
    template<typename T>
    static constexpr bool is_io_tuple_v = requires { typename T::is_io_tuple; };

    template<typename T, typename... Ts>
    static bool write_final(
        T const& t, Ts&... args
    ) AC_DEDUCE_NOEXCEPT_REQUIRES_AND_RETURN( //
        t.write_final(args...)
    )

    template<typename T, typename... Ts>
    static bool read(
        T& t, Ts&&... args
    ) AC_DEDUCE_NOEXCEPT_REQUIRES_AND_RETURN( //
        t.read_final(args...)
    )
};

template<typename T>
concept IO_Tuple = serialization_access::is_io_tuple_v<T>;

template<typename T>
bool write_final(Device auto& od, Format auto& fmt, T const& t)
    AC_DEDUCE_NOEXCEPT_REQUIRES_AND_RETURN( //
        serialization_access::write_final(t, od, fmt)
    )

template<typename T>
bool read_final(Device auto& id, Format auto& fmt, T& t)
    AC_DEDUCE_NOEXCEPT_REQUIRES_AND_RETURN( //
        serialization_access::read(t, id, fmt)
    )

} // namespace ac::io
