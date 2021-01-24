// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/assert.hpp>
#include <actl/io/io.hpp>
#include <actl/std/string.hpp>
#include <cstring>

namespace ac::io {

template <mode_t Mode, class Char, bool = is_out<Mode>>
class out_string : public device<Mode, Char> {
public:
    explicit out_string(const std::basic_string<Char>& s) : s_{s} {}

protected:
    const std::basic_string<Char>& s_;
};

template <mode_t Mode, class Char>
class out_string<Mode, Char, true> : public device<Mode, Char> {
public:
    static_assert((Mode & app) > 0, "only append is supported now");

    explicit out_string(std::basic_string<Char>& s) : s_{s} {
        if constexpr (trunc == (Mode & trunc))
            s = {};
    }

    index write(Char c) {
        s_ += c;
        return 1;
    }

    index write(const cspan<Char>& src) {
        s_.append(src.data(), static_cast<size_t>(src.size()));
        return src.size();
    }

    void flush() {}

protected:
    std::basic_string<Char>& s_;
};

template <mode_t Mode, class Char, bool = is_in<Mode>>
class in_string : public out_string<Mode, Char> {
public:
    using out_string<Mode, Char>::out_string;
};

template <mode_t Mode, class Char>
class in_string<Mode, Char, true> : public out_string<Mode, Char> {
protected:
    using base_t = out_string<Mode, Char>;
    using base_t::s_;
    index pos_ = 0;

public:
    using base_t::base_t;

    cspan<Char> input_data() const {
        return {s_.data() + pos_, s_.data() + s_.size()};
    }

    Char peek() {
        auto i = static_cast<size_t>(pos_);
        return i < s_.size() ? s_[i] : Char{};
    }

    Char get() {
        Char c = peek();
        ++pos_;
        return c;
    }

    index read(const span<Char>& dst) {
        index count = std::min(dst.size(), static_cast<index>(s_.size()) - pos_);
        std::memcpy(dst.data(), s_.data() + pos_, static_cast<size_t>(count));
        pos_ += count;
        return count;
    }

    void move(index offset) {
        pos_ += offset;
        ACTL_ASSERT(0 <= pos_ && pos_ <= static_cast<index>(s_.size()));
    }

    bool eof() const {
        return static_cast<index>(s_.size()) < pos_;
    }
};

template <mode_t Mode, class Char = char>
using string = in_string<Mode, Char>;

} // namespace ac::io
