/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/io/io.hpp>
#include <actl/io/util/unbuffered.hpp>
#include <actl/traits/container_traits.hpp>
#include <algorithm>
#include <cstring>

namespace ac::io {

template <mode_t Mode, class T, class Char, bool = is_out<Mode>>
class out_memory : public device<Mode, Char> {
    using C = add_const_if_t<!is_out<Mode>, Char>;

public:
    template <class... Ts>
    explicit out_memory(Ts&&... xs) : data_{std::forward<Ts>(xs)...}, ptr_{std::begin(data_)} {}

    span<C> available() const { return {ptr_, end()}; }

    void move(index offset) {
        ptr_ += offset;
        ACTL_ASSERT(std::begin(data_) <= ptr_ && ptr_ <= end());
    }

    bool eof() const { return end() < ptr_; }

protected:
    C* end() const { return std::end(data_); }

    T data_;
    C* ptr_;
};

template <mode_t Mode, class T, class Char>
class out_memory<Mode, T, Char, true> : public out_memory<Mode, T, Char, false> {
protected:
    using base_t = out_memory<Mode, T, Char, false>;
    using base_t::end;
    using base_t::ptr_;

public:
    using out_memory<Mode, T, Char, false>::out_memory;

    index write(Char c) {
        if (ptr_ >= end()) return 0;
        *ptr_++ = c;
        return 1;
    }

    index write(const cspan<Char>& src) {
        index count = std::min(src.size(), end() - ptr_);
        std::memcpy(ptr_, src.data(), static_cast<size_t>(count) * sizeof(Char));
        ptr_ += count;
        return count;
    }

    void flush() {}
};

template <mode_t Mode, class T, class Char, bool = is_in<Mode>>
class in_memory : public out_memory<Mode, T, Char> {
public:
    using out_memory<Mode, T, Char>::out_memory;
};

template <mode_t Mode, class T, class Char>
class in_memory<Mode, T, Char, true> : public out_memory<Mode, T, Char> {
protected:
    using base_t = out_memory<Mode, T, Char>;
    using base_t::end;
    using base_t::ptr_;

public:
    using base_t::base_t;

    Char peek() { return ptr_ < end() ? *ptr_ : Char{}; }

    Char get() {
        Char c = peek();
        ++ptr_;
        return c;
    }

    index read(const span<Char>& dst) {
        index count = std::min(dst.size(), end() - ptr_);
        std::memcpy(dst.data(), ptr_, static_cast<size_t>(count));
        ptr_ += count;
        return count;
    }
};

template <mode_t Mode, class T = span<add_const_if_t<!is_out<Mode>, default_char_t<Mode>>>>
using memory = unbuffered<in_memory<Mode, T, value_type_t<T>>>;

}  // namespace ac::io
