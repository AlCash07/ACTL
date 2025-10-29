// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/AC_ASSERT.hpp>
#include <actl/io/device/mode.hpp>
#include <actl/io/io.hpp>
#include <actl/std/string.hpp>
#include <actl/utility/index.hpp>
#include <cstring>

namespace ac::io {

template<mode_t Mode, class Char, bool = is_out<Mode>>
class out_string : public device<Mode, Char> {
public:
    explicit out_string(std::basic_string<Char> const& s) : m_s{s} {}

protected:
    std::basic_string<Char> const& m_s;
};

template<mode_t Mode, class Char>
class out_string<Mode, Char, true> : public device<Mode, Char> {
public:
    static_assert((Mode & app) > 0, "only append is supported now");

    explicit out_string(std::basic_string<Char>& s) : m_s{s} {
        if constexpr (trunc == (Mode & trunc))
            s = {};
    }

    size_t write(Char c) {
        m_s += c;
        return 1;
    }

    size_t write(cspan<Char> src) {
        m_s.append(src.data(), src.size());
        return src.size();
    }

    void flush() {}

protected:
    std::basic_string<Char>& m_s;
};

template<mode_t Mode, class Char, bool = is_in<Mode>>
class in_string : public out_string<Mode, Char> {
public:
    using out_string<Mode, Char>::out_string;
};

template<mode_t Mode, class Char>
class in_string<Mode, Char, true> : public out_string<Mode, Char> {
protected:
    using base_t = out_string<Mode, Char>;
    using base_t::m_s;
    size_t m_pos = 0;

public:
    using base_t::base_t;

    cspan<Char> input_buffer() const {
        return {m_s.data() + m_pos, m_s.data() + m_s.size()};
    }

    Char peek() {
        return m_pos < m_s.size() ? m_s[m_pos] : Char{};
    }

    Char get() {
        Char c = peek();
        ++m_pos;
        return c;
    }

    size_t read(span<Char> dst) {
        size_t count =
            std::min(dst.size(), static_cast<size_t>(m_s.size() - m_pos));
        std::memcpy(dst.data(), m_s.data() + m_pos, count);
        m_pos += count;
        return count;
    }

    void move(index offset) {
        m_pos = static_cast<size_t>(static_cast<index>(m_pos) + offset);
        AC_ASSERT(m_pos <= m_s.size());
    }

    bool eof() const {
        return m_s.size() < m_pos;
    }
};

template<mode_t Mode, class Char = char>
using string = in_string<Mode, Char>;

} // namespace ac::io
