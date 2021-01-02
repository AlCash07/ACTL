// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <actl/io/io.hpp>
#include <cstdio>
#include <cstring>

namespace ac::io {

template <mode_t Mode, class Char, bool = is_in<Mode>>
class in_file : public device<Mode, Char> {
    static constexpr const char* mode_str[14] = {
        "r", "rb", "w", "wb", "w+", "w+b", "a", "ab", "a+", "a+b", "a", "ab", "r+", "r+b",
    };

public:
    explicit in_file(std::FILE* file, bool own = false) : file_{file}, own_{own} {
        ACTL_ASSERT(file);
    }

    explicit in_file(const char* filename)
        : in_file{std::fopen(filename, mode_str[(Mode & 0xF) - 2]), true} {}

    ~in_file() {
        if (own_)
            std::fclose(file_);
    }

    bool eof() const {
        return std::feof(file_) != 0;
    }

protected:
    std::FILE* file_;
    bool own_;
};

template <mode_t Mode, class Char>
class in_file<Mode, Char, true> : public in_file<Mode, Char, false> {
public:
    using in_file<Mode, Char, false>::in_file;

    Char peek() {
        Char c = get();
        move(-1);
        return c;
    }

    Char get() {
        int c = std::fgetc(this->file_);
        return c == EOF ? Char{} : static_cast<Char>(c);
    }

    index read(const span<Char>& dst) {
        size_t res{};
        if constexpr (is_line_buffered<Mode>) {
            if (std::fgets(dst.data(), static_cast<int>(dst.size()), this->file_))
                res = std::strlen(dst.data());
        } else {
            res =
                std::fread(dst.data(), sizeof(Char), static_cast<size_t>(dst.size()), this->file_);
        }
        return static_cast<index>(res);
    }

    void move(index offset) {
        int res = std::fseek(this->file_, offset * static_cast<index>(sizeof(Char)), SEEK_CUR);
        ACTL_ASSERT(res == 0);
    }
};

template <mode_t Mode, class Char = std::byte, bool = is_out<Mode>>
class file : public in_file<Mode, Char> {
public:
    using in_file<Mode, Char>::in_file;
};

template <mode_t Mode, class Char>
class file<Mode, Char, true> : public in_file<Mode, Char> {
public:
    using in_file<Mode, Char>::in_file;

    index write(Char c) {
        return std::fputc(static_cast<int>(c), this->file_) != EOF ? 1 : 0;
    }

    index write(const cspan<Char>& src) {
        return static_cast<index>(
            std::fwrite(src.data(), sizeof(Char), static_cast<size_t>(src.size()), this->file_));
    }

    void flush() {
        std::fflush(this->file_);
    }

    ~file() {
        flush();
    }
};

}  // namespace ac::io
