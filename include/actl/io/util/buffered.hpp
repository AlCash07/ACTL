/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/assert.hpp>
#include <actl/io/io.hpp>
#include <actl/macros.hpp>
#include <actl/range/algorithm.hpp>
#include <array>

namespace ac::io {

template <class Device, int BufferSize = 1 << 10, bool Read = is_in<Device::mode>>
class buffered_reader : public Device {
public:
    using Device::Device;

protected:
    using Char = typename Device::char_type;

    std::array<Char, BufferSize> data_;
    Char* ptr_ = data_.begin();
    Char* end_ = Read ? data_.begin() : data_.end();
};

template <class Device, int BS>
class buffered_reader<Device, BS, true> : public buffered_reader<Device, BS, false> {
protected:
    using base_t = buffered_reader<Device, BS, false>;
    using base_t::end_;
    using base_t::ptr_;
    using typename base_t::Char;

    void underflow() {
        ptr_ = this->data_.begin();
        end_ = ptr_ + Device::read(this->data_);
    }

public:
    using buffered_reader<Device, BS, false>::buffered_reader;

    Char peek() {
        if (ptr_ >= end_) underflow();
        return ptr_ < end_ ? *ptr_ : {};
    }

    Char get() {
        Char c = peek();
        ++ptr_;
        return c;
    }

    index read(span<Char> dst) {
        Char* dstPtr = dst.data();
        index count = dst.size();
        index available = end_ - ptr_;
        if (count <= available) {
            std::copy_n(ptr_, count, dstPtr);
            ptr_ += count;
            return count;
        }
        index res = available;
        dstPtr = std::copy_n(ptr_, available, dstPtr);
        count -= available;
        index remainder = count % BS;
        if (remainder < count) {
            res += Device::read({dstPtr, count - remainder});
            dstPtr += count - remainder;
        }
        underflow();
        smin(remainder, end_ - ptr_);
        if (0 < remainder) {
            res += remainder;
            std::copy_n(ptr_, remainder, dstPtr);
            ptr_ += remainder;
        }
        return res;
    }

    template <class T>
    index read(till<Char, T> dst) {
        Char* dstPtr = dst.data();
        Char* last = dst.end();
        while (ptr_ < end_) {
            Char* ptr = ptr_;
            Char* end = std::min(end_, ptr_ + (last - dstPtr));
            while (ptr_ != end && !dst.terminator(*ptr_)) *dstPtr++ = *ptr_++;
            if (ptr == ptr_) break;
            underflow();
        }
        return dst.size() - (last - dstPtr);
    }

    void move(index offset) {
        // TODO: support move fully along the underlying device, not just along the buffer.
        ptr_ += offset;
        ACTL_ASSERT(this->data_.data() <= ptr_ && ptr_ < end_);
    }

    bool eof() { return ptr_ > end_; }
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
    using base_t::end_;
    using base_t::ptr_;
    using typename base_t::Char;

    void overflow() {
        Device::write({data_, ptr_});
        ptr_ = data_;
    }

    void write_impl(span<const Char> src) {
        const Char* srcPtr = src.data();
        index count = src.size();
        index available = end_ - ptr_;
        if (EXPECT_TRUE(count < available)) {
            if (count == 1) {
                *ptr_++ = *srcPtr;
            } else {
                ptr_ = copy(src, ptr_);
            }
        } else {
            std::copy_n(srcPtr, available, ptr_);
            Device::write(data_);
            srcPtr += available;
            count -= available;
            index remainder = count % BS;
            if (remainder < count) {
                Device::write({srcPtr, count - remainder});
                srcPtr += count - remainder;
            }
            if (0 < remainder) {
                ptr_ = std::copy_n(srcPtr, remainder, data_.data());
            }
        }
    }

public:
    void put(Char arg) {
        *ptr_++ = arg;
        if (EXPECT_FALSE(ptr_ == end_)) overflow();
    }

    index write(span<const Char> src) {
        if constexpr (is_line_buffered<Mode>) {
            const Char* last = src.end();
            while (last != src.data() && last[-1] != '\n') --last;
            if (last != src.data()) {
                write_impl({src.data(), last});
                flush();
            }
            write_impl({last, src.end()});
        } else {
            write_impl(src);
        }
        return src.size();
    }

    void write_fill(Char c, index count) {
        index chunk_size = end_ - ptr_;
        if (EXPECT_TRUE(count < chunk_size)) {
            ptr_ = std::fill_n(ptr_, count, c);
        } else {
            std::fill_n(ptr_, chunk_size, c);
            Device::write(data_);
            count -= chunk_size;
            std::fill_n(data_.data(), std::min(count, BS), c);
            for (index n = count / BS; n > 0; --n) {
                Device::write(data_);
            }
            ptr_ = data_.data() + count % BS;
        }
    }

    void flush() {
        overflow();
        Device::flush();
    }
};

}  // namespace ac::io
