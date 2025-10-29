// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/io/device/mode.hpp>
#include <actl/io/io.hpp>
#include <actl/meta/type_traits.hpp>
#include <actl/range/span.hpp>
#include <actl/utility/index.hpp>
#include <algorithm>
#include <cstring>

namespace ac::io {

template<mode_t Mode, class Char, bool = is_out<Mode>>
class out_memory : public device<Mode, Char> {
    using C = add_const_if_t<!is_out<Mode>, Char>;

public:
    explicit out_memory(span<C> data)
        : m_data{std::move(data)}, m_ptr{m_data.begin()} {}

    void move(index offset) {
        m_ptr += offset;
        AC_ASSERT(m_data.begin() <= m_ptr && m_ptr <= end());
    }

    bool eof() const {
        return end() < m_ptr;
    }

protected:
    C* end() const {
        return m_data.end();
    }

    span<C> const m_data;
    C* m_ptr;
};

template<mode_t Mode, class Char>
class out_memory<Mode, Char, true> : public out_memory<Mode, Char, false> {
protected:
    using base_t = out_memory<Mode, Char, false>;
    using base_t::end;
    using base_t::m_ptr;

public:
    using base_t::base_t;

    span<Char> output_buffer() const {
        return {m_ptr, end()};
    }

    size_t write(Char c) {
        if (m_ptr >= end())
            return 0;
        *m_ptr++ = c;
        return 1;
    }

    size_t write(cspan<Char> src) {
        size_t count = std::min(src.size(), static_cast<size_t>(end() - m_ptr));
        std::memcpy(m_ptr, src.data(), count * sizeof(Char));
        m_ptr += count;
        return count;
    }

    void flush() {}
};

template<mode_t Mode, class Char, bool = is_in<Mode>>
class in_memory : public out_memory<Mode, Char> {
public:
    using out_memory<Mode, Char>::out_memory;
};

template<mode_t Mode, class Char>
class in_memory<Mode, Char, true> : public out_memory<Mode, Char> {
protected:
    using base_t = out_memory<Mode, Char>;
    using base_t::end;
    using base_t::m_ptr;

public:
    using base_t::base_t;

    cspan<Char> input_buffer() const {
        return {m_ptr, end()};
    }

    Char peek() {
        return m_ptr < end() ? *m_ptr : Char{};
    }

    Char get() {
        Char c = peek();
        ++m_ptr;
        return c;
    }

    size_t read(span<Char> dst) {
        size_t count = std::min(dst.size(), static_cast<size_t>(end() - m_ptr));
        std::memcpy(dst.data(), m_ptr, count);
        m_ptr += count;
        return count;
    }
};

template<mode_t Mode, class Char = char>
using memory = in_memory<Mode, Char>;

} // namespace ac::io
