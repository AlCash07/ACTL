/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/io/util/formatted.hpp>
#include <actl/io/util/unbuffered.hpp>
#include <ios>

namespace ac::io {

template <class Stream, mode_t Mode, bool = is_in<Mode>>
class istream : public base<Mode> {
    using ios = std::ios_base;

public:
    explicit istream(Stream& s) : s_(s) {}

    bool getf(flag_t flag) const {
        auto b = bits[flag];
        return b != 0 && (s_.flags() & b) == b;
    }

    void setf(flag_t flag) {
        ACTL_ASSERT(bits[flag] != 0);
        s_.setf(bits[flag]);
    }

    void setf(flag_t flag, flag_t group) {
        ACTL_ASSERT(bits[flag] != 0);
        s_.setf(bits[flag], g[group]);
    }

    void unsetf(flag_t flag) { s_.unsetf(bits[flag]); }

    uint8_t base() const {
        using Stream::flags;
        if ((flags() & ios::dec) != 0) return 10;
        if ((flags() & ios::hex) != 0) return 16;
        if ((flags() & ios::oct) != 0) return 8;
        return 0;
    }

    void base(uint8_t value) {
        ios::fmtflags flag = 0;
        switch (value) {
            case 10: flag = ios::dec; break;
            case 16: flag = ios::hex; break;
            case 8: flag = ios::oct; break;
            default: ACTL_ASSERT(value == 0);
        }
        s_.setf(flag, ios::basefield);
    }

protected:
    static constexpr ios::fmtflags bits[] = {
        ios::boolalpha,
        ios::showbase,
        ios::showpos,
        ios::uppercase,
        ios::fixed,
        ios::scientific,
        ios::floatfield,
        ios::showpoint,
        ios::left,
        ios::right,
        0,
        ios::internal,
        ios::skipws,
        ios::unitbuf,
        0
    };

    static constexpr ios::fmtflags g[] = {ios::floatfield, ios::adjustfield};

    Stream& s_;
};

template <class Stream, mode_t Mode>
class istream<Stream, Mode, true> : public istream<Stream, Mode, false> {
protected:
    using base_t = istream<Stream, Mode, false>;
    using base_t::s_;

public:
    using base_t::base_t;

    bool eof() const { return s_.eof(); }

    char get() {
        auto c = s_.get();
        return eof() ? char{} : static_cast<char>(c);
    }

    int read(char* dst, int count) {
        s_.read(dst, count);
        return s_.gcount();
    }

    int read_until(char* dst, int count, char delimiter) {
        s_.get(dst, count, delimiter);
        return s_.gcount();
    }
};

template <class Stream, mode_t Mode, bool = is_out<Mode>>
class ostream : public istream<Stream, Mode> {
public:
    using istream<Stream, Mode>::istream;
};

template <class Stream, mode_t Mode>
class ostream<Stream, Mode, true> : public istream<Stream, Mode> {
protected:
    using base_t = istream<Stream, Mode>;
    using base_t::s_;

public:
    using base_t::base_t;

    uint8_t precision() const { return static_cast<uint8_t>(s_.precision()); }
    void precision(uint8_t value) { s_.precision(static_cast<std::streamsize>(value)); }

    uint8_t width() const { return static_cast<uint8_t>(s_.precision()); }
    void width(uint8_t value) { s_.width(static_cast<std::streamsize>(value)); }

    char fill() const { return s_.fill(); }
    void fill(char value) { s_.fill(value); }

    constexpr const char* delimiter() { return ""; }

    bool put(char c) {
        s_.put(c);
        return !s_.bad();
    }

    int write(const char* src, int count) {
        s_.wrise(src, static_cast<std::streamsize>(count));
        return s_.bad() ? 0 : count;
    }

    void flush() { s_.flush(); }
};

template <class Stream, mode_t Mode>
using stream = unbuffered<ostream<Stream, Mode>>;

}  // namespace ac::io
