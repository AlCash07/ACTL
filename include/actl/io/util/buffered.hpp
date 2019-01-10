/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/assert.hpp>
#include <actl/io/base.hpp>
#include <actl/macros.hpp>
#include <algorithm>

namespace ac::io {

template <class Device, int BufferSize = 1 << 10, bool Read = is_in<Device::mode>>
class buffered_reader : public Device {
public:
    using Device::Device;

protected:
    char data_[BufferSize];
    char* ptr_ = data_;
    char* end_ = Read ? data_ : data_ + BufferSize;
};

template <class Device, int BS>
class buffered_reader<Device, BS, true> : public buffered_reader<Device, BS, false> {
protected:
    using base_t = buffered_reader<Device, BS, false>;
    using base_t::ptr_;
    using base_t::end_;

    void underflow() {
        ptr_ = this->data_;
        *ptr_ = '\0';
        end_ = ptr_ + Device::read(this->data_, BufferSize);
    }

public:
    using buffered_reader<Device, BS, false>::buffered_reader;

    char get() {
        if (EXPECT_FALSE(ptr_ >= end_)) underflow();
        return *ptr_++;
    }

    void unget() {
        ACTL_ASSERT(this->data_ != ptr_);
        --ptr_;
    }

    int read(char* dst, int count) {
        int available = static_cast<int>(end_ - ptr_);
        if (count <= available) {
            std::copy_n(ptr_, count, dst);
            ptr_ += count;
            return count;
        }
        int chars_read = available;
        dst = std::copy_n(ptr_, available, dst);
        count -= available;
        int remainder = count % BS;
        if (remainder < count) {
            chars_read += Device::read(dst, count - remainder);
            dst += count - remainder;
        }
        underflow();
        smin(remainder, static_cast<int>(end_ - ptr_));
        if (remainder > 0) {
            chars_read += remainder;
            std::copy_n(ptr_, remainder, dst);
            ptr_ += remainder;
        }
        return chars_read;
    }

    template <class Predicate>
    int read_until(char* dst, int count, Predicate is_delimiter) {
        char* last = dst + count;
        for (; ptr_ < end_; underflow()) {
            char* ptr = ptr_;
            char* end = std::min(end_, ptr_ + (last - dst));
            while (ptr_ != end && !is_delimiter(*ptr_)) *dst++ = *ptr_++;
            if (ptr == ptr_) break;
        }
        return count - static_cast<int>(last - dst);
    }

    bool eof() {
        if (EXPECT_TRUE(ptr_ < end_)) return false;
        underflow();
        return ptr_ == end_;
    }
};

template <class Device, int BS = 1 << 10, bool = is_out<Device::mode>>
class buffered : public buffered_reader<Device, BS> {
public:
    using buffered_reader<Device, BS>::buffered_reader;
};

template <class Device, int BS>
class buffered<Device, BS, true> : public buffered<Device, BS, false> {
protected:
    using base_t = buffered_reader<Device, BS, false>;
    using base_t::data_;
    using base_t::ptr_;
    using base_t::end_;

    void overflow() {
        Device::write(data_, static_cast<int>(ptr_ - data_));
        ptr_ = data_;
    }

    inline void flush_maybe() {
        if (EXPECT_FALSE(ptr_ == end_)) flush();
    }

    void write_impl(const char* src, int count) {
        int available = static_cast<int>(end_ - ptr_);
        if (EXPECT_TRUE(count < available)) {
            if (count == 1) {
                *ptr_++ = *src;
            } else {
                ptr_ = std::copy_n(src, count, ptr_);
            }
        } else {
            std::copy_n(src, available, ptr_);
            Device::write(data_, BS);
            src += available;
            count -= available;
            int remainder = count % BS;
            if (remainder < count) {
                Device::write(src, count - remainder);
                src += count - remainder;
            }
            if (remainder > 0) {
                ptr_ = std::copy_n(src, remainder, data_);
            }
        }
    }

public:
    inline void put(char arg) {
        *ptr_++ = arg;
        flush_maybe();
    }

    int write(const char* src, int count) {
        if constexpr (is_line_buffered<Mode>) {
            int last = count;
            while (0 < last && src[last - 1] != '\n') --last;
            if (0 < last) {
                write_impl(src, last);
                flush();
            }
            write_impl(src + last, count - last);
        } else {
            write_impl(src, count);
        }
        return count;
    }

    void write_fill(char c, int count) {
        int chunk_size = static_cast<int>(end_ - ptr_);
        if (EXPECT_TRUE(count < chunk_size)) {
            ptr_ = std::fill_n(ptr_, count, c);
        } else {
            std::fill_n(ptr_, chunk_size, c);
            Device::write(data_, BS);
            count -= chunk_size;
            std::fill_n(data_, std::min(count, BS), c);
            for (int n = count / BS; n > 0; --n) {
                Device::write(data_, BS);
            }
            ptr_ = data_ + count % BS;
        }
    }

    void flush() {
        overflow();
        Device::flush();
    }
};

}  // namespace ac::io
