/***************************************************************************************************
 * Copyright 2017 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/assert.hpp>
#include <actl/io/device/buffered_output_device.hpp>
#include <iterator>
#include <string>

namespace ac {

class text_output_device {
public:
    using category = text_io_tag;

    explicit text_output_device() { compute_base_power(); }

    // Minimum width of a single element to write (except char).
    uint32_t get_width() const { return width_; }
    void set_width(uint32_t value) { width_ = value; }

    // Character prepended to an element until set width is reached.
    char get_fill() const { return fill_; }
    void set_fill(char value) { fill_ = value; }

    // Base for integer and floating point numbers.
    uint32_t get_base() const { return base_; }
    void set_base(uint32_t value) {
        ACTL_ASSERT(2 <= value && value <= 36);
        base_ = value;
        compute_base_power();
    }

    // Number of digits after the decimal point.
    uint32_t get_precision() const { return precision_; }
    void set_precision(uint32_t value) {
        precision_ = value;
        compute_base_power();
    }

    // Delimiter automatically inserted between elements that are not strings or characters.
    const char* get_delimiter() const { return delimiter_.data(); }
    void set_delimiter(const char* value) { delimiter_ = value; }

    unsigned long long get_base_power() const { return base_power_; }

protected:
    void compute_base_power() {
        base_power_ = 1;
        for (uint32_t i = 0; i < precision_; ++i) base_power_ *= base_;
    }

    bool        separate_  = false;
    uint32_t    width_     = 0;
    char        fill_      = ' ';
    uint32_t    base_      = 10;
    uint32_t    precision_ = 9;
    std::string delimiter_ = " ";

    unsigned long long base_power_;
};

template <uint32_t BufferSize = 1 << 11>
class text_buffered_output_device : public text_output_device,
                                    public buffered_output_device<BufferSize> {
public:
    using category = text_io_tag;

    // Maximum possible length of a string representing primitive type except huge double values.
    static constexpr uint32_t MaxLength = 1 << 7;

    char* get_end() { return auxiliary_ + MaxLength; }

    inline void write_char(char arg) {
        separate_ = false;
        buffered_output_device<BufferSize>::write_char(arg);
    }

    inline int write_value(const char* begin) {
        int chars_written = separate_ ? write_delimiter() : 0;
        chars_written += write_string(begin, static_cast<size_t>(get_end() - begin));
        separate_ = true;
        return chars_written;
    }

    inline int write_string(const char* arg, size_t length, bool check_width = true) {
        separate_ = false;
        const int count = static_cast<int>(length);
        int chars_written = count;
        if (EXPECT_FALSE(check_width && count < width_)) {
            struct fill_iterator : std::iterator<std::input_iterator_tag, char> {
                fill_iterator(char fill) : fill{fill} {}
                char operator * () const { return fill; }
                void operator ++ () const {}
                void operator += (uint32_t) const {}
                const char fill;
            };
            chars_written += this->write_string_impl(fill_iterator(fill_), width_ - count);
        }
        return chars_written + this->write_string_impl(arg, count);
    }

    inline int write_delimiter() {
        return write_string(delimiter_.c_str(), delimiter_.size(), false);
    }

private:
    char auxiliary_[MaxLength];
};

template <bool Binary, uint32_t BufferSize>
using output_device_base_t = std::conditional_t<Binary, buffered_output_device<BufferSize>,
                                                text_buffered_output_device<BufferSize>>;

}  // namespace ac
