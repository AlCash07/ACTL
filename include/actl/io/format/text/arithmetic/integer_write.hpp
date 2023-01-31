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

namespace ac::io {

namespace detail {

template <uint8_t Size>
class int_string
{
public:
    void set_size(size_t size)
    {
        data_[Size] = static_cast<char>(size);
    }

    void set_start(char const* x)
    {
        set_size(static_cast<size_t>(data_ + Size - x));
    }

    span<char, Size> available()
    {
        return {data_, Size};
    }

    explicit operator cspan<char>() const
    {
        size_t const size = static_cast<size_t>(data_[Size]);
        return {ranges::end(data_) - 1 - size, size};
    }

private:
    char data_[Size + 1];
};

template <uint8_t Size>
bool write_final(Device auto& od, Format auto& fmt, int_string<Size> const& x)
{
    return write_final(od, fmt, cspan<char>{x});
}

} // namespace detail

template <class Int>
    requires(
        std::integral<Int> && !std::same_as<Int, char> &&
        !std::same_as<Int, bool>)
auto encode(TextFormat auto& fmt, Int x)
{
    using UInt = std::make_unsigned_t<Int>;
    UInt base = fmt.base;
    if (base == 0)
        base = 10;
    detail::int_string<
        1 + detail::digit_count(std::numeric_limits<UInt>::max(), UInt{2})>
        s;
    auto last = s.available().end();
    if constexpr (std::is_signed_v<Int>)
    {
        if (x < 0)
        {
            last =
                detail::uitoa(last, fmt, ~static_cast<UInt>(x) + UInt{1}, base);
            *--last = '-';
        }
        else
        {
            last = detail::uitoa(last, fmt, static_cast<UInt>(x), base);
            if (fmt.getf(flag::showpos))
                *--last = '+';
        }
    }
    else
    {
        last = detail::uitoa(last, fmt, x, base);
    }
    s.set_start(last);
    return s;
}

} // namespace ac::io
