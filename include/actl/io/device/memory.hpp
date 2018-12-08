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

namespace ac::io {

template <mode_t Mode, bool = is_out<Mode>>
class out_memory : public base<Mode> {
    explicit out_memory(const char* begin, const char* end)
        : begin_{begin}, ptr_{begin}, end_{end} {
        ACTL_ASSERT(begin < end);
    }

protected:
    const char* begin_;
    const char* ptr_;
    const char* end_;
};

template <mode_t Mode>
class out_memory<Mode, true> : public base<Mode> {
public:
    explicit out_memory(char* begin, const char* end) : begin_{begin}, ptr_{begin}, end_{end} {
        ACTL_ASSERT(begin < end);
    }

    bool put(char c) {
        bool ok = ptr_ < end_;
        if (ok) *ptr_++ = c;
        return ok;
    }

    int write(const char* src, int count) {
        smin(count, static_cast<int>(end_ - ptr_));
        std::memcpy(ptr_, src, static_cast<size_t>(count));
        ptr_ += count;
        return count;
    }

    void flush() {}

protected:
    char* begin_;
    char* ptr_;
    const char* end_;
};

template <mode_t Mode, bool = is_in<Mode>>
class in_memory : public out_memory<Mode> {
public:
    using out_memory<Mode>::out_memory;
};

template <mode_t Mode>
class in_memory<Mode, true> : public out_memory<Mode> {
protected:
    using base_t = out_memory<Mode>;
    using base_t::end_;
    using base_t::ptr_;

public:
    using base_t::base_t;

    bool eof() const { return ptr_ == end_; }

    char get() { return eof() ? char{} : *ptr_++; }

    void unget() {
        ACTL_ASSERT(this->begin_ != ptr_);
        --ptr_;
    }

    int read(char* dst, int count) {
        smin(count, static_cast<int>(end_ - ptr_));
        std::memcpy(dst, ptr_, static_cast<size_t>(count));
        ptr_ += count;
        return count;
    }
};

template <mode_t Mode>
using memory = unbuffered<in_memory<Mode>>;

}  // namespace ac::io
