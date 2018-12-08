/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/assert.hpp>
#include <actl/functions.hpp>
#include <actl/io/util/unbuffered.hpp>
#include <cstring>
#include <string>

namespace ac::io {

template <mode_t Mode, bool = is_out<Mode>>
class out_string : public base<Mode> {
public:
    explicit out_string(const std::string& s) : s_{s} {}

protected:
    const std::string& s_;
};

template <mode_t Mode>
class out_string<Mode, true> : public base<Mode> {
public:
    static_assert((Mode & app) > 0, "only append is supported now");

    explicit out_string(std::string& s) : s_{s} {
        if constexpr (trunc == (Mode & trunc)) s.clear();
    }

    bool put(char c) { s_ += c; }

    int write(const char* src, int count) { s_.append(src, static_cast<size_t>(count)); }

    void flush() {}

protected:
    std::string& s_;
};

template <mode_t Mode, bool = is_in<Mode>>
class in_string : public out_string<Mode> {
public:
    using out_string<Mode>::out_string;
};

template <mode_t Mode>
class in_string<Mode, true> : public out_string<Mode> {
protected:
    using base_t = out_string<Mode>;
    using base_t::s_;
    int pos_ = 0;

public:
    using base_t::base_t;

    bool eof() const { return static_cast<size_t>(pos_) == s_.size(); }

    char get() { return eof() ? char{} : s_[pos_]++; }

    void unget() {
        ACTL_ASSERT(pos_ > 0);
        --pos_;
    }

    int read(char* dst, int count) {
        smin(count, static_cast<int>(s_.size() - pos_));
        std::memcpy(dst, s_.data() + pos_, static_cast<size_t>(count));
        pos_ += count;
        return count;
    }
};

template <mode_t Mode>
using string = unbuffered<in_string<Mode>>;

}  // namespace ac::io
