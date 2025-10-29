// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/AC_ASSERT.hpp>
#include <actl/io/io.hpp>
#include <actl/operation/scalar/basic_math/all.hpp>
#include <actl/range/operation/copy.hpp>

namespace ac::io {

template<typename Device, typename Buffer, bool Read = is_in<Device::mode>>
class buffered_reader : public Device {
public:
    // TODO: design a way to pass constructor arguments to the buffer.
    using Device::Device;

protected:
    Buffer m_buf;
    char_t<Device>* m_ptr = ranges::data(m_buf);
};

template<typename Device, typename Buffer>
class buffered_reader<Device, Buffer, true>
    : public buffered_reader<Device, Buffer, false> {
protected:
    using Char = char_t<Device>;
    using base_t = buffered_reader<Device, Buffer, false>;
    using base_t::m_ptr;

    Char* m_end = m_ptr;

    void underflow() {
        m_ptr = ranges::data(this->m_buf);
        m_end = m_ptr + Device::read(this->m_buf);
    }

public:
    using base_t::base_t;

    cspan<Char> input_buffer() const {
        return {m_ptr, m_end};
    }

    Char peek() {
        return m_ptr < m_end ? *m_ptr : Char{};
    }

    Char get() {
        Char c = peek();
        move(1);
        return c;
    }

    size_t read(span<Char> dst) {
        Char* dstPtr = dst.data();
        size_t count = dst.size();
        size_t available = static_cast<size_t>(m_end - m_ptr);
        if (count < available) {
            std::copy_n(m_ptr, count, dstPtr);
            m_ptr += count;
            return count;
        }
        size_t res = available;
        dstPtr = std::copy_n(m_ptr, available, dstPtr);
        count -= available;
        size_t remainder = count % ranges::size(base_t::m_buf);
        if (remainder < count) {
            res += Device::read({dstPtr, count - remainder});
            dstPtr += count - remainder;
        }
        underflow();
        min(inout{remainder}, m_end - m_ptr);
        if (0 < remainder) {
            res += remainder;
            std::copy_n(m_ptr, remainder, dstPtr);
            move(remainder);
        }
        return res;
    }

    void move(index offset) {
        // TODO: support move fully along the underlying device, not just along
        // the buffer.
        m_ptr += offset;
        AC_ASSERT(ranges::data(this->m_buf) <= m_ptr);
        if (m_ptr == m_end)
            underflow();
    }

    bool eof() {
        return m_end < m_ptr;
    }
};

template<
    typename Device,
    typename Buffer = char_t<Device>[1 << 10],
    bool = is_out<Device::mode>>
class buffered : public buffered_reader<Device, Buffer> {
public:
    using buffered_reader<Device, Buffer>::buffered_reader;
};

template<typename Device, typename Buffer>
class buffered<Device, Buffer, true> : public buffered<Device, Buffer, false> {
protected:
    using Char = char_t<Device>;
    using base_t = buffered<Device, Buffer, false>;
    using base_t::m_buf;
    using base_t::m_ptr;

    void overflow() {
        Device::write({ranges::data(m_buf), m_ptr});
        m_ptr = ranges::data(m_buf);
    }

public:
    using base_t::base_t;

    span<Char> output_buffer() const {
        return {m_ptr, ranges::end(m_buf)};
    }

    size_t write(Char c) {
        *m_ptr = c;
        move(1);
        return 1;
    }

    size_t write(span<Char const> src) {
        Char const* srcPtr = src.data();
        size_t count = src.size();
        size_t available = static_cast<size_t>(ranges::end(m_buf) - m_ptr);
        if (count < available) {
            if (count == 1)
                *m_ptr++ = *srcPtr;
            else
                m_ptr = ranges::copy(m_ptr, src);
        } else {
            std::copy_n(srcPtr, available, m_ptr);
            Device::write(m_buf);
            srcPtr += available;
            count -= available;
            size_t remainder = count % ranges::size(m_buf);
            if (remainder < count) {
                Device::write({srcPtr, count - remainder});
                srcPtr += count - remainder;
            }
            if (0 < remainder)
                m_ptr = std::copy_n(srcPtr, remainder, ranges::data(m_buf));
        }
        return src.size();
    }

    void move(index offset) {
        m_ptr += offset;
        AC_ASSERT(ranges::data(m_buf) <= m_ptr);
        if (m_ptr == ranges::end(m_buf))
            overflow();
    }

    void flush() {
        overflow();
        Device::flush();
    }

    ~buffered() {
        overflow();
    }
};

} // namespace ac::io
