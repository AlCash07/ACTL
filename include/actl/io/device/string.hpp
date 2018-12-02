/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/assert.hpp>
#include <actl/functions.hpp>
#include <actl/io/io.hpp>
#include <cstring>
#include <string>

namespace ac::io {

template <mode_t Mode, bool = in_v<Mode>, bool = out_v<Mode>>
class string;

template <mode_t Mode>
class string<Mode, true, false> : public base<Mode> {
public:
    explicit string(const char* begin, const char* end) : begin_{begin}, ptr_{begin}, end_{end} {}

    explicit string(const char* s) : string(s, s + std::strlen(s)) {}

    explicit string(const std::string& s) : string(s.data(), s.data() + s.size()) {}

    bool eof() const { return ptr_ == end_; }

    char get() { return eof() ? '\0' : *ptr_++; }

    void unget() {
        ACTL_ASSERT(begin_ != ptr_);
        --ptr_;
    }

    int read(char* dst, int count) {
        smin(count, static_cast<int>(end_ - ptr_));
        std::memcpy(dst, ptr_, static_cast<size_t>(count));
        ptr_ += count;
    }

protected:
    const char* begin_;
    const char* ptr_;
    const char* end_;
};

template <mode_t Mode>
class string<Mode, false, true> : public base<Mode> {
public:
    explicit string(std::string& s) : str_(s) {
        if constexpr (0 < (Mode & out)) s.clear();
    }

    bool put(char c) { str_ += c; }

    int write(const char* src, int count) { str_.append(src, static_cast<size_t>(count)); }

    void flush() {}

protected:
    std::string& str_;
};

}  // namespace ac::io
