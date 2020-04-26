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

namespace ac::io {

template <class Device, class Buffer, bool Read = is_in<Device::mode>>
class buffered_reader : public Device {
public:
    // TODO: design a way to pass constructor arguments to the buffer.
    using Device::Device;

protected:
    Buffer buf_;
    char_t<Device>* ptr_ = std::data(buf_);
};

template <class Device, class Buffer>
class buffered_reader<Device, Buffer, true> : public buffered_reader<Device, Buffer, false> {
protected:
    using Char = char_t<Device>;
    using base_t = buffered_reader<Device, Buffer, false>;
    using base_t::ptr_;

    Char* end_ = ptr_;

    void underflow() {
        ptr_ = std::data(this->buf_);
        end_ = ptr_ + Device::read(this->buf_);
    }

public:
    using base_t::base_t;

    cspan<Char> input_data() const { return {ptr_, end_}; }

    Char peek() { return ptr_ < end_ ? *ptr_ : Char{}; }

    Char get() {
        Char c = peek();
        move(1);
        return c;
    }

    index read(const span<Char>& dst) {
        Char* dstPtr = dst.data();
        index count = dst.size();
        index available = end_ - ptr_;
        if (count < available) {
            std::copy_n(ptr_, count, dstPtr);
            ptr_ += count;
            return count;
        }
        index res = available;
        dstPtr = std::copy_n(ptr_, available, dstPtr);
        count -= available;
        index remainder = count % std::size(base_t::buf_);
        if (remainder < count) {
            res += Device::read({dstPtr, count - remainder});
            dstPtr += count - remainder;
        }
        underflow();
        math::min(inplace(remainder), end_ - ptr_);
        if (0 < remainder) {
            res += remainder;
            std::copy_n(ptr_, remainder, dstPtr);
            move(remainder);
        }
        return res;
    }

    void move(index offset) {
        // TODO: support move fully along the underlying device, not just along the buffer.
        ptr_ += offset;
        ACTL_ASSERT(std::data(this->buf_) <= ptr_);
        if (ptr_ == end_) underflow();
    }

    bool eof() { return end_ < ptr_; }
};

template <class Device, class Buffer = char_t<Device>[1 << 10], bool = is_out<Device::mode>>
class buffered : public buffered_reader<Device, Buffer> {
public:
    using buffered_reader<Device, Buffer>::buffered_reader;
};

template <class Device, class Buffer>
class buffered<Device, Buffer, true> : public buffered<Device, Buffer, false> {
protected:
    using Char = char_t<Device>;
    using base_t = buffered_reader<Device, Buffer, false>;
    using base_t::buf_;
    using base_t::ptr_;

    void overflow() {
        Device::write({std::data(buf_), ptr_});
        ptr_ = std::data(buf_);
    }

    void write_impl(const cspan<Char>& src) {
        const Char* srcPtr = src.data();
        index count = src.size();
        index available = std::end(buf_) - ptr_;
        if (count < available) {
            if (count == 1) {
                *ptr_++ = *srcPtr;
            } else {
                ptr_ = copy(src, ptr_);
            }
        } else {
            std::copy_n(srcPtr, available, ptr_);
            Device::write(buf_);
            srcPtr += available;
            count -= available;
            index remainder = count % std::size(buf_);
            if (remainder < count) {
                Device::write({srcPtr, count - remainder});
                srcPtr += count - remainder;
            }
            if (0 < remainder) {
                ptr_ = std::copy_n(srcPtr, remainder, std::data(buf_));
            }
        }
    }

public:
    using base_t::base_t;

    span<Char> output_data() const { return {ptr_, std::end(buf_)}; }

    index write(Char arg) {
        *ptr_ = arg;
        move(1);
        return 1;
    }

    index write(const cspan<Char>& src) {
        if constexpr (is_line_buffered<base_t::mode>) {
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

    void move(index offset) {
        ptr_ += offset;
        ACTL_ASSERT(std::data(buf_) <= ptr_);
        if (ptr_ == std::end(buf_)) overflow();
    }

    void flush() {
        overflow();
        Device::flush();
    }

    ~buffered() { overflow(); }
};

}  // namespace ac::io
