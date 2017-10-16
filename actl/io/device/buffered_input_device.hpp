/***************************************************************************************************
 * Copyright 2017 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <algorithm>
#include <actl/io/io_traits.hpp>
#include <actl/macros.hpp>

namespace ac {

/**
 * Base class for buffered input devices. Buffer itself is managed by the derived classes.
 */
class buffered_input_device {
public:
    using category = binary_io_tag;

    buffered_input_device(buffered_input_device const&) = delete;
    buffered_input_device& operator = (buffered_input_device const&) = delete;

    virtual ~buffered_input_device() {}

    inline char read_char() {
        if (EXPECT_FALSE(head_ >= tail_)) fill_buffer();
        return *head_++;
    }

    int read_string(char* arg, int count) {
        return read_string_impl(arg, count, [](const char*, const char* last) { return last; });
    }

    // Can be called only once directly after successfully reading a character.
    void put_back() { --head_; }

    bool eof() {
        if (EXPECT_TRUE(head_ < tail_)) return false;
        fill_buffer();
        return head_ == tail_;
    }

protected:
    buffered_input_device(const char* head, const char* tail) : head_{head}, tail_{tail} {}

    virtual void fill_buffer() = 0;

    template <class RangeFunction>
    int read_string_impl(char* arg, int count, RangeFunction get_last) {
        int chars_read = 0;
        for (; head_ < tail_; fill_buffer()) {
            std::ptrdiff_t chunk_size =
                get_last(head_, std::min(tail_, head_ + count - chars_read)) - head_;
            arg = std::copy_n(head_, chunk_size, arg);
            head_ += chunk_size;
            chars_read += chunk_size;
            if (chunk_size == 0 || head_ < tail_) break;
        }
        return chars_read;
    }

    const char* head_;
    const char* tail_;
};

}  // namespace ac
