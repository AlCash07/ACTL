/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/io/base.hpp>
#include <actl/traits/ctype.hpp>
#include <cstring>

namespace ac::io {

inline bool is_one_of(char c, const char* s) { return std::strchr(s, c) != nullptr; }

template <class Predicate>
class skip_t {
    Predicate pred;

public:
    explicit constexpr skip_t(Predicate pred) : pred(pred) {}

    constexpr bool operator()(char c) const { return pred(c); }
};

inline constexpr auto skip() { return skip_t(is_space); }

inline auto skip(char c) { return skip_t([c](char x) { return x == c; }); }

inline auto skip(const char* s) { return skip_t([s](char c) { return is_one_of(c, s); }); }

template <class Predicate>
inline auto skip(Predicate pred) { return skip_t(pred); }

inline constexpr auto ws = skip();  // skip whitespace

template <class Device, class Predicate>
inline bool read(io::itext& code, Device& rd, io::skip_t<Predicate> skip) {
    for (char& c = code.last; skip(code.last); c = rd.get());
    return true;
}

}  // namespace ac::io
