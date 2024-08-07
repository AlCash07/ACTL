// Copyright 2019 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/io/argument/byte_span.hpp>
#include <actl/io/format/text/arithmetic/detail/digit_count.hpp>
#include <actl/io/format/text/arithmetic/detail/uitoa.hpp>
#include <actl/io/format/text/text.hpp>
#include <actl/numeric/algorithm/binary_pow.hpp>
#include <algorithm>
#include <cmath>
#include <memory>

namespace ac::io {

namespace detail {

class float_string {
public:
    span<char> reserve(size_t size) {
        ptr_ = std::make_unique<char[]>(size);
        return {ptr_.get(), size};
    }

    explicit operator cspan<char>() const {
        return data_;
    }

    void set_span(cspan<char> value) {
        data_ = value;
    }

private:
    std::unique_ptr<char[]> ptr_;
    cspan<char> data_;
};

bool write_final(Device auto& od, Format auto& fmt, float_string const& x) {
    return write_final(od, fmt, cspan<char>{x});
}

} // namespace detail

template<std::floating_point Float>
auto encode(TextFormat auto& fmt, Float x) {
    detail::float_string res;
    span<char> s;
    char* first;
    char sign{};
    if (std::signbit(x)) {
        sign = '-';
        x = -x;
    } else {
        if (fmt.getf(flag::showpos))
            sign = '+';
    }
    if (std::isnan(x)) {
        s = res.reserve(4);
        first = s.begin() + 1;
        std::memcpy(first, fmt.getf(flag::uppercase) ? "NAN" : "nan", 3);
    } else if (std::isinf(x)) {
        s = res.reserve(4);
        first = s.begin() + 1;
        std::memcpy(first, fmt.getf(flag::uppercase) ? "INF" : "inf", 3);
    } else {
        using UInt = unsigned long long;
        UInt base = fmt.base;
        size_t precision = fmt.precision;
        auto base_power = binary_pow(base, precision);
        auto integer_part = static_cast<UInt>(x);
        auto fractional_part =
            static_cast<UInt>((x - integer_part) * base_power + Float{0.5});
        if (fractional_part >= base_power) {
            ++integer_part;
            fractional_part = 0;
        }
        s = res.reserve(
            detail::digit_count(
                std::numeric_limits<UInt>::max(), base < 10 ? UInt{2} : UInt{10}
            ) +
            1 + std::max(size_t{0}, precision)
        );
        first = s.end() - precision;
        if (0 < precision) {
            std::fill(
                first, detail::uitoa(s.end(), fmt, fractional_part, base), '0'
            );
        }
        if (0 < precision || fmt.getf(flag::showpoint))
            *--first = '.';
        first = detail::uitoa(first, fmt, integer_part, base);
    }
    if (sign)
        *--first = sign;
    res.set_span({first, s.end()});
    return res;
}

} // namespace ac::io
