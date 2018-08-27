/***************************************************************************************************
 * Copyright 2017 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/io/io_traits.hpp>
#include <actl/macros.hpp>
#include <algorithm>
#include <cstdint>

namespace ac {

/**
 * Base class for buffered output devices.
 * @tparam BufferSize size of the allocated buffer.
 */
template <uint32_t BufferSize = 1 << 11>
class buffered_output_device {
public:
    using category = binary_io_tag;

    buffered_output_device(buffered_output_device const&) = delete;
    buffered_output_device& operator = (buffered_output_device const&) = delete;

    virtual ~buffered_output_device() {}

    inline void write_char(char arg) {
        *tail_++ = arg;
        flush_maybe();
    }

    int write_string(const char* arg, int count) { return write_string_impl(arg, count); }

    void flush() {
        flush_buffer(static_cast<int>(tail_ - buffer_));
        tail_ = buffer_;
    }

protected:
    char  buffer_[BufferSize];
    char* tail_;

    buffered_output_device() : tail_{buffer_} {}

    virtual void flush_buffer(int count) = 0;

    inline void flush_maybe() {
        if (EXPECT_FALSE(tail_ == buffer_ + BufferSize)) flush();
    }

    template <class InputIterator>
    int write_string_impl(InputIterator arg, int count) {
        int chars_written = count;
        if (EXPECT_TRUE(tail_ + count < buffer_ + BufferSize)) {
            if (count == 1) {
                *tail_++ = *arg;
            } else {
                tail_ = std::copy_n(arg, count, tail_);
            }
        } else {
            int chunk_size = static_cast<int>(buffer_ + BufferSize - tail_);
            for (;; chunk_size = BufferSize) {
                if (chunk_size > count) chunk_size = count;
                tail_ = std::copy_n(arg, chunk_size, tail_);
                flush_maybe();
                if ((count -= chunk_size) == 0) break;
                arg += chunk_size;
            }
        }
        return chars_written;
    }
};

}  // namespace ac
