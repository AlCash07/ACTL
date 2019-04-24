/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/io/io.hpp>
#include <actl/traits/ctype.hpp>
#include <actl/types.hpp>
#include <cstring>

namespace ac::io {

inline bool is_one_of(char c, czstring s) { return std::strchr(s, c) != nullptr; }

template <class Predicate>
class skip_t {
public:
    explicit constexpr skip_t(Predicate pred) : pred_{pred} {}

    constexpr bool operator()(char c) const { return pred_(c); }

private:
    Predicate pred_;
};

inline constexpr auto skip() { return skip_t{is_space}; }

inline auto skip(char c) {
    return skip_t{[c](char x) { return x == c; }};
}

inline auto skip(czstring s) {
    return skip_t{[s](char c) { return is_one_of(c, s); }};
}

template <class Predicate>
inline auto skip(Predicate pred) {
    return skip_t{pred};
}

inline constexpr auto ws = skip();  // skip whitespace

template <class Device, class Format, class Predicate>
inline bool deserialize(Device& id, Format& fmt, skip_t<Predicate> skip, text) {
    //    for (char& c = id.last; skip(id.last); c = id.get());
    return true;
}

}  // namespace ac::io
