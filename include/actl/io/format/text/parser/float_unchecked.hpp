// Copyright 2020 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/io/format/text/arithmetic/detail/digit_count.hpp>
#include <actl/io/format/text/parser/sign.hpp>
#include <actl/io/format/text/parser/uint_unchecked.hpp>
#include <actl/numeric/algorithm/binary_pow.hpp>
#include <actl/std/utility.hpp>

namespace ac::io {

template <class Float>
class float_unchecked_parser
{
    using UInt = unsigned long long;

    enum class states
    {
        empty,
        sign,
        integral,
        point
    };

    states state = states::empty;
    sign_parser<true> sign;
    UInt const base;
    size_t const max_length;
    Float x = 0;
    Float power = 1;

    auto parse_int(cspan<char> s)
    {
        uint_unchecked_parser<UInt> parser{base};
        size_t length =
            parser.parse(s.size() <= max_length ? s : s.first(max_length));
        return std::pair{parser.value(), length};
    }

public:
    explicit float_unchecked_parser(uint8_t base)
        : base{base == 0 ? 10 : UInt{base}}
        , max_length{
              detail::digit_count(
                  std::numeric_limits<UInt>::max(), this->base) -
              1}
    {}

    size_t parse(cspan<char> s)
    {
        size_t i = 0;
        switch (state)
        {
            case states::empty:
                i += sign.parse(s);
                state = states::sign;
                if (i == s.size())
                    break;
            case states::sign:
                if (auto [p, length] = parse_int(s.subspan(i)); length == 0)
                    break;
                else
                {
                    x = static_cast<Float>(p);
                    i += length;
                    state = states::integral;
                }
            case states::integral:
                while (i != s.size())
                {
                    auto [p, length] = parse_int(s.subspan(i));
                    if (length == 0)
                    {
                        if (s[i] == '.')
                        {
                            ++i;
                            state = states::point;
                            break;
                        }
                        else
                            return i;
                    }
                    x = x * binary_pow(base, length) + p;
                    i += length;
                }
            case states::point:
                while (i != s.size())
                {
                    auto [p, length] = parse_int(s.subspan(i));
                    if (length == 0)
                        break;
                    power /= binary_pow(base, length);
                    x += p * power;
                    i += length;
                }
        }
        return i;
    }

    bool ready() const
    {
        return state != states::empty && state != states::sign;
    }

    Float value() const
    {
        return sign.value(x);
    }
};

} // namespace ac::io
