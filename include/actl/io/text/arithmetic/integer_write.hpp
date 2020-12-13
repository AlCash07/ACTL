// Copyright 2019 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/io/text/arithmetic/detail/digit_count.hpp>
#include <actl/io/text/arithmetic/detail/uitoa.hpp>
#include <actl/io/text/text.hpp>
#include <actl/util/type_traits.hpp>

namespace ac::io {

namespace detail {

template <uint8_t Size>
class int_string {
public:
    void set_size(index size) { data_[Size] = static_cast<char>(size); }
    void set_start(const char* x) { set_size(data_ + Size - x); }

    span<char, Size> available() { return {data_, Size}; }

    explicit operator cspan<char>() const {
        const index size = data_[Size];
        return {std::end(data_) - 1 - size, size};
    }

private:
    char data_[Size + 1];
};

template <class D, class F, uint8_t Size>
inline index write_final(D& od, F& fmt, const int_string<Size>& x) {
    return write_final(od, fmt, cspan<char>{x});
}

}  // namespace detail

template <class Format, class Int, enable_int_if_text<Format> = 0,
          enable_int_if<std::is_integral_v<Int> && !std::is_same_v<Int, char> &&
                        !std::is_same_v<Int, bool>> = 0>
auto encode(Format& fmt, Int x) {
    using UInt = std::make_unsigned_t<Int>;
    UInt base = fmt.base;
    if (base == 0) base = 10;
    detail::int_string<1 + detail::digit_count(std::numeric_limits<UInt>::max(), UInt{2})> s;
    auto last = s.available().end();
    if constexpr (std::is_signed_v<Int>) {
        if (x < 0) {
            last = detail::uitoa(last, fmt, ~static_cast<UInt>(x) + UInt{1}, base);
            *--last = '-';
        } else {
            last = detail::uitoa(last, fmt, static_cast<UInt>(x), base);
            if (fmt.getf(flags::showpos)) *--last = '+';
        }
    } else {
        last = detail::uitoa(last, fmt, x, base);
    }
    s.set_start(last);
    return s;
}

}  // namespace ac::io
