/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/functions.hpp>
#include <actl/io/util/unbuffered.hpp>
#include <algorithm>
#include <cstring>

namespace ac::io {

template <mode_t Mode, class Char, bool = is_out<Mode>>
class out_memory : public device<Mode, Char> {
public:
    explicit out_memory(span<const Char> data) : data_{data}, ptr_{data_.begin()} {}

protected:
    span<const Char> data_;
    const Char* ptr_;
};

template <mode_t Mode, class Char>
class out_memory<Mode, Char, true> : public device<Mode, Char> {
public:
    explicit out_memory(span<Char> data) : data_{data}, ptr_{data_.begin()} {}

    bool put(Char c) {
        bool ok = ptr_ < data_.end();
        if (ok) *ptr_++ = c;
        return ok;
    }

    index write(span<const Char> src) {
        index count = std::min(src.size(), data_.end() - ptr_);
        std::memcpy(ptr_, src.data(), static_cast<size_t>(count) * sizeof(Char));
        ptr_ += count;
        return count;
    }

    void flush() {}

protected:
    span<Char> data_;
    Char* ptr_;
};

template <mode_t Mode, class Char, bool = is_in<Mode>>
class in_memory : public out_memory<Mode, Char> {
public:
    using out_memory<Mode, Char>::out_memory;
};

template <mode_t Mode, class Char>
class in_memory<Mode, Char, true> : public out_memory<Mode, Char> {
protected:
    using base_t = out_memory<Mode, Char>;
    using base_t::data_;
    using base_t::ptr_;

public:
    using base_t::base_t;

    Char peek() { return ptr_ < data_.end() ? *ptr_ : Char{}; }

    Char get() {
        Char c = peek();
        ++ptr_;
        return c;
    }

    index read(span<Char> dst) {
        index count = std::min(dst.size(), data_.end() - ptr_);
        std::memcpy(dst.data(), ptr_, static_cast<size_t>(count));
        ptr_ += count;
        return count;
    }

    void move(index offset) {
        ptr_ += offset;
        ACTL_ASSERT(data_.begin() <= ptr_ && ptr_ <= data_.end());
    }

    bool eof() const { return ptr_ > data_.end(); }
};

template <mode_t Mode, class Char = default_char_t<Mode>>
using memory = unbuffered<in_memory<Mode, Char>>;

}  // namespace ac::io
