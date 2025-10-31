// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/io/device/mode.hpp>
#include <actl/io/io.hpp>
#include <cstdio>
#include <cstring>

namespace ac::io {

template<mode_t Mode, typename Char, bool = is_in<Mode>>
class in_file : public device<Mode, Char> {
    static constexpr char const* mode_str[14] = {
        "r",
        "rb",
        "w",
        "wb",
        "w+",
        "w+b",
        "a",
        "ab",
        "a+",
        "a+b",
        "a",
        "ab",
        "r+",
        "r+b"
    };

public:
    explicit in_file(std::FILE* file, bool own = false)
        : m_file{file}, m_own{own} {
        AC_ASSERT(file);
    }

    explicit in_file(char const* filename)
        : in_file{std::fopen(filename, mode_str[(Mode & 0xF) - 2]), true} {}

    ~in_file() {
        if (m_own)
            std::fclose(m_file);
    }

    std::FILE* get() const {
        return m_file;
    }

    bool eof() const {
        return std::feof(m_file) != 0;
    }

protected:
    std::FILE* m_file;
    bool m_own;
};

template<mode_t Mode, typename Char>
class in_file<Mode, Char, true> : public in_file<Mode, Char, false> {
public:
    using in_file<Mode, Char, false>::in_file;

    Char peek() {
        Char c = get();
        move(-1);
        return c;
    }

    Char get() {
        int c = std::fgetc(this->m_file);
        return c == EOF ? Char{} : static_cast<Char>(c);
    }

    size_t read(span<Char> chars) {
        return std::fread(
            chars.data(), sizeof(Char), chars.size(), this->m_file
        );
    }

    void move(index offset) {
        int res = std::fseek(
            this->m_file, offset * static_cast<index>(sizeof(Char)), SEEK_CUR
        );
        AC_ASSERT(res == 0);
    }
};

template<mode_t Mode, typename Char = std::byte, bool = is_out<Mode>>
class file : public in_file<Mode, Char> {
public:
    using in_file<Mode, Char>::in_file;
};

template<mode_t Mode, typename Char>
class file<Mode, Char, true> : public in_file<Mode, Char> {
public:
    using in_file<Mode, Char>::in_file;

    size_t write(Char c) {
        return std::fputc(static_cast<int>(c), this->m_file) != EOF ? 1 : 0;
    }

    size_t write(span<Char const> chars) {
        return std::fwrite(
            chars.data(), sizeof(Char), chars.size(), this->m_file
        );
    }

    void flush() {
        std::fflush(this->m_file);
    }

    ~file() {
        flush();
    }
};

} // namespace ac::io
