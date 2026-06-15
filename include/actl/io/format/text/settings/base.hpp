// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/AC_ASSERT.hpp>
#include <actl/utility/index.hpp>

namespace ac::io {

// Integer and real numbers base.
class base_t {
public:
    struct is_manipulator;

    explicit constexpr base_t() = default;

    template<typename T>
    explicit constexpr base_t(T t) : value{static_cast<uint8_t>(t)} {
        AC_ASSERT(t == 0 || 1 < t && t <= 36);
    }

    base_t& operator=(size_t t) {
        return *this = base_t{t};
    }

    constexpr operator uint8_t() const {
        return value;
    }

private:
    uint8_t value = 10;
};

inline constexpr base_t decimal{10};
inline constexpr base_t hex{16};
inline constexpr base_t octal{8};
inline constexpr base_t detect_base{0};

} // namespace ac::io
