// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/io/device/mode.hpp>
#include <actl/io/io.hpp>
#include <actl/range/span.hpp>
#include <algorithm>
#include <cstring>

namespace ac::io {

template <mode_t Mode, class Char, bool = is_out<Mode>>
class out_memory : public device<Mode, Char>
{
    using C = add_const_if_t<!is_out<Mode>, Char>;

public:
    explicit out_memory(span<C> data)
        : data_{std::move(data)}, ptr_{data_.begin()}
    {}

    void move(index offset)
    {
        ptr_ += offset;
        ACTL_ASSERT(data_.begin() <= ptr_ && ptr_ <= end());
    }

    bool eof() const
    {
        return end() < ptr_;
    }

protected:
    C* end() const
    {
        return data_.end();
    }

    span<C> const data_;
    C* ptr_;
};

template <mode_t Mode, class Char>
class out_memory<Mode, Char, true> : public out_memory<Mode, Char, false>
{
protected:
    using base_t = out_memory<Mode, Char, false>;
    using base_t::end;
    using base_t::ptr_;

public:
    using base_t::base_t;

    span<Char> output_data() const
    {
        return {ptr_, end()};
    }

    size_t write(Char c)
    {
        if (ptr_ >= end())
            return 0;
        *ptr_++ = c;
        return 1;
    }

    size_t write(cspan<Char> src)
    {
        size_t count = std::min(src.size(), static_cast<size_t>(end() - ptr_));
        std::memcpy(ptr_, src.data(), count * sizeof(Char));
        ptr_ += count;
        return count;
    }

    void flush() {}
};

template <mode_t Mode, class Char, bool = is_in<Mode>>
class in_memory : public out_memory<Mode, Char>
{
public:
    using out_memory<Mode, Char>::out_memory;
};

template <mode_t Mode, class Char>
class in_memory<Mode, Char, true> : public out_memory<Mode, Char>
{
protected:
    using base_t = out_memory<Mode, Char>;
    using base_t::end;
    using base_t::ptr_;

public:
    using base_t::base_t;

    cspan<Char> input_data() const
    {
        return {ptr_, end()};
    }

    Char peek()
    {
        return ptr_ < end() ? *ptr_ : Char{};
    }

    Char get()
    {
        Char c = peek();
        ++ptr_;
        return c;
    }

    size_t read(span<Char> dst)
    {
        size_t count = std::min(dst.size(), static_cast<size_t>(end() - ptr_));
        std::memcpy(dst.data(), ptr_, count);
        ptr_ += count;
        return count;
    }
};

template <mode_t Mode, class Char = char>
using memory = in_memory<Mode, Char>;

} // namespace ac::io
