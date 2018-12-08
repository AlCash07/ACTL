/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/io/util/buffered.hpp>
#include <cstdio>
#include <cstring>

namespace ac::io {

template <mode_t Mode, bool = is_in<Mode>>
class in_file : public base<Mode> {
    static constexpr const char* mode_str[14] = {
        "r", "rb", "w", "wb", "w+", "w+b", "a", "ab", "a+", "a+b", "a", "ab", "r+", "r+b",
    };

public:
    explicit in_file(std::FILE* file, bool own = false) : file_{file}, own_{own} {
        ACTL_ASSERT(file);
    }

    explicit in_file(const char* filename)
        : in_file(std::fopen(filename, mode_str[(Mode & 0xF) - 2]), true) {}

    ~in_file() {
        if (own_) std::fclose(file_);
    }

    bool eof() const { return std::feof(file_) != 0; }

protected:
    std::FILE* file_;
    bool       own_;
};

template <mode_t Mode>
class in_file<Mode, true> : public in_file<Mode, false> {
public:
    using in_file<Mode, false>::in_file;

    char get() {
        auto c = std::fgetc(this->file_);
        return static_cast<char>(c == EOF ? '\0' : c);
    }

    void unget() { std::fseek(this->file_, -1, SEEK_CUR); }

    int read(char* dst, int count) {
        *dst = char{};
        if constexpr (is_line_buffered<Mode>) {
            return std::fgets(dst, count, this->file_) ? static_cast<int>(std::strlen(dst)) : 0;
        } else {
            return static_cast<int>(std::fread(dst, 1, static_cast<size_t>(count), this->file_));
        }
    }
};

template <mode_t Mode, bool = is_out<Mode>>
class file : public in_file<Mode> {
public:
    using in_file<Mode>::in_file;
};

template <mode_t Mode>
class file<Mode, true> : public in_file<Mode> {
public:
    using in_file<Mode>::in_file;

    bool put(char c) { return std::fputc(c, this->file_) != EOF; }

    int write(const char* src, int count) {
        return static_cast<int>(std::fwrite(src, 1, static_cast<size_t>(count), this->file_));
    }

    void flush() { std::fflush(this->file_); }
};

}  // namespace ac::io
