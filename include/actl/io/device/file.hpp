/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/io/io.hpp>
#include <cstdio>
#include <cstring>

namespace ac::io {

template <mode_t Mode, bool = in_v<Mode>>
class freader : public base<Mode> {
    static constexpr const char* mode_str[14] = {
        "r", "rb", "w", "wb", "w+", "w+b", "a", "ab", "a+", "a+b", "a", "ab", "r+", "r+b",
    };

public:
    explicit freader(std::FILE* file, bool own = false) : file_{file}, own_{own} {}

    explicit freader(const char* filename)
        : freader(std::fopen(filename, mode_str[(Mode & 0xF) - 2]), true) {}

    ~freader() {
        if (own_) std::fclose(file_);
    }

    bool eof() const { return std::feof(file_); }

protected:
    std::FILE* file_;
    bool       own_;
};

template <mode_t Mode>
class freader<Mode, true> : public freader<Mode, false> {
public:
    using freader<Mode, false>::freader;

    char get() {
        auto c = std::fgetc(this->file_);
        return static_cast<char>(c == EOF ? '\0' : c);
    }

    void unget() { std::fseek(this->file_, -1, SEEK_CUR); }

    int read(char* dst, int count) {
        *dst = '\0';
        if constexpr (line_buffered_v<Mode>) {
            return std::fgets(dst, count, this->file_) ? static_cast<int>(std::strlen(dst)) : 0;
        } else {
            return static_cast<int>(std::fread(dst, 1, count, this->file_));
        }
    }
};

template <mode_t Mode, bool = out_v<Mode>>
class fwriter : public freader<Mode> {
public:
    using freader<Mode>::freader;
};

template <mode_t Mode>
class fwriter<Mode, true> : public fwriter<Mode, false> {
public:
    using fwriter<Mode, false>::fwriter;

    bool put(char c) { return std::fputc(c, this->file_) != EOF; }

    int write(const char* src, int count) {
        static_cast<int>(std::fwrite(src, 1, static_cast<size_t>(count), this->file_));
    }

    void flush() { std::fflush(this->file_); }
};

template <mode_t Mode = 0>
class input_file : public freader<Mode | in> {
    using base_t = freader<Mode | in>;

public:
    using base_t::base_t;

    explicit input_file() : base_t(stdin) {}
};

template <mode_t Mode = 0>
class output_file : public fwriter<Mode | (out_v<Mode> ? 0 : out)> {
    using base_t = fwriter<Mode | (out_v<Mode> ? 0 : out)>;

public:
    using base_t::base_t;

    explicit output_file() : base_t(stdout) {}
};

}  // namespace ac::io
