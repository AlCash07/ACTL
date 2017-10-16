/***************************************************************************************************
 * Copyright 2017 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/assert.hpp>
#include <actl/io/device/buffered_input_device.hpp>

namespace ac {

class text_input_device {
public:
    using category = text_io_tag;

    // Base for integer and floating point numbers.
    uint32_t get_base() const { return base_; }
    void set_base(uint32_t value) {
        ACTL_ASSERT(2 <= value && value <= 36);
        base_ = value;
    }

protected:
    uint32_t base_ = 10;
};

class text_buffered_input_device : public text_input_device, public buffered_input_device {
public:
    using category = text_io_tag;

    template <class UnaryPredicate>
    int read_string(char* arg, int limit, UnaryPredicate is_terminator) {
        --limit;  // Reserve space for null character.
        int chars_read =
            read_string_impl(arg, limit, [is_terminator](const char* first, const char* last) {
                return std::find_if(first, last, is_terminator);
            });
        arg[chars_read] = '\0';
        return chars_read;
    }

protected:
    text_buffered_input_device(const char* head, const char* tail)
        : buffered_input_device(head, tail) {}
};

template <bool Binary>
using input_device_base_t =
    std::conditional_t<Binary, buffered_input_device, text_buffered_input_device>;

}  // namespace ac
