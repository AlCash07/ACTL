/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/assert.hpp>
#include <actl/bit.hpp>
#include <actl/io/text/decorator/adjusted.hpp>
#include <actl/io/util/unbuffered.hpp>
#include <ios>

namespace ac::io {

template <class Stream>
class ios_format {
    using ios = std::ios_base;

public:
    using char_type = typename Stream::char_type;

    explicit ios_format(Stream& s) : s_{s} {}

    bool getf(flag_t flag) const {
        auto b = bits[flag];
        return b != 0 && has_bits(s_.flags(), b);
    }

    void setf(flag_t flag) {
        ACTL_ASSERT(bits[flag] != 0);
        s_.setf(bits[flag]);
    }

    void unsetf(flag_t flag) { s_.unsetf(bits[flag]); }

    void setf(flag_t flag, flag_t group) {
        ACTL_ASSERT(bits[flag] != 0);
        s_.setf(bits[flag], g[group]);
    }

    uint8_t base() const {
        if ((s_.flags() & ios::dec) != 0) return 10;
        if ((s_.flags() & ios::hex) != 0) return 16;
        if ((s_.flags() & ios::oct) != 0) return 8;
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

    index precision() const { return s_.precision(); }
    void precision(index value) { s_.precision(value); }

    index width() const { return s_.precision(); }
    void width(index value) { s_.width(value); }

    char_type fill() const { return s_.fill(); }
    void fill(char_type value) { s_.fill(value); }

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
        ios::skipws,
        ios::unitbuf,
        0
    };

    static constexpr ios::fmtflags g[] = {ios::floatfield, ios::adjustfield};

    Stream& s_;
};

template <class Stream>
struct format_traits<ios_format<Stream>> {
    using tag = adjusted_tag<text_tag>;
};

template <mode_t Mode, class Stream, bool = is_in<Mode>>
class istream : public device<Mode, typename Stream::char_type> {
public:
    explicit istream(Stream& s) : s_{s} {}

protected:
    Stream& s_;
};

template <mode_t Mode, class Stream>
class istream<Mode, Stream, true> : public istream<Mode, Stream, false> {
protected:
    using base_t = istream<Mode, Stream, false>;
    using base_t::s_;

public:
    using char_type = typename Stream::char_type;

    using base_t::base_t;

    char_type peek() {
        auto c = s_.peek();
        return c == typename Stream::traits_type::eof() ? char_type{} : static_cast<char_type>(c);
    }

    char_type get() {
        auto c = s_.get();
        return c == typename Stream::traits_type::eof() ? char_type{} : static_cast<char_type>(c);
    }

    index read(const span<char_type>& dst) {
        s_.read(dst.data(), dst.size());
        return s_.gcount();
    }

    index read_until(till<char_type, char_type> dst) {
        s_.get(dst.data(), dst.size(), dst.terminator.pred);
        return s_.gcount();
    }

    void move(index offset) {
        s_.seekg(offset, std::ios_base::cur);
        ACTL_ASSERT(!s_.fail());
    }

    bool eof() const { return s_.eof(); }
};

template <mode_t Mode, class Stream, bool = is_out<Mode>>
class ostream : public istream<Mode, Stream> {
public:
    using istream<Mode, Stream>::istream;
};

template <mode_t Mode, class Stream>
class ostream<Mode, Stream, true> : public istream<Mode, Stream> {
protected:
    using base_t = istream<Mode, Stream>;
    using base_t::s_;

public:
    using typename base_t::char_type;

    using base_t::base_t;

    index put(char_type c) {
        s_.put(c);
        return s_.bad() ? 0 : 1;
    }

    index write(const cspan<char_type>& src) {
        s_.wrise(src.data(), src.size());
        return s_.bad() ? 0 : src.size();
    }

    void flush() { s_.flush(); }
};

template <mode_t Mode, class Stream>
using stream = unbuffered<ostream<Mode, Stream>>;

}  // namespace ac::io
