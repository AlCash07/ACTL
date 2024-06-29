// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

namespace ac::io {

// Number of digits after the decimal point.
class precision_t {
public:
    explicit constexpr precision_t() = default;

    explicit constexpr precision_t(size_t x) : value{x} {}

    precision_t& operator=(size_t x) {
        return *this = precision_t{x};
    }

    constexpr operator size_t() const {
        return value;
    }

private:
    size_t value = 6;
};

} // namespace ac::io
