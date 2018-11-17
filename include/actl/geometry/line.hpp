/***************************************************************************************************
 * Copyright 2017 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/geometry/point.hpp>
#include <cstdint>

namespace ac {

namespace endpoint {

enum : uint8_t { free = 0, closed = 1, open = 2 };

inline constexpr uint8_t combine(uint8_t start, uint8_t end) {
    return static_cast<uint8_t>(start | end << 2);
}
inline constexpr uint8_t start(uint8_t combined) { return static_cast<uint8_t>(combined & 3); }
inline constexpr uint8_t end(uint8_t combined) { return static_cast<uint8_t>(combined >> 2); }

}  // namespace endpoint

namespace line_kind {

enum : uint8_t {
    free                = endpoint::combine(endpoint::free,   endpoint::free),
    closed_ray          = endpoint::combine(endpoint::closed, endpoint::free),
    open_ray            = endpoint::combine(endpoint::open,   endpoint::free),
    closed_segment      = endpoint::combine(endpoint::closed, endpoint::closed),
    open_closed_segment = endpoint::combine(endpoint::open,   endpoint::closed),
    open_segment        = endpoint::combine(endpoint::open,   endpoint::open)
};

inline constexpr bool is_valid(uint8_t kind) {
    uint8_t start = endpoint::start(kind);
    return endpoint::end(kind) <= start && start < 3;
}

template <uint8_t Kind>
struct static_kind {
    static_kind(uint8_t kind = Kind) { (*this) = kind; }

    static constexpr uint8_t kind() { return Kind; }

    void operator = (uint8_t kind) { ACTL_ASSERT(Kind == kind); }
};

template <class Device, uint8_t K>
inline bool read(Device&, static_kind<K>&) { return true; }

template <class Device, uint8_t K>
inline int write(Device&, const static_kind<K>&) { return 0; }

class any_kind {
public:
    any_kind(uint8_t kind = free) { (*this) = kind; }

    void operator = (uint8_t kind) {
        ACTL_ASSERT(is_valid(kind));
        kind_ = kind;
    }

    uint8_t kind() const { return kind_; }

private:
    uint8_t kind_;
};

template <class Device>
inline bool read(Device& in, any_kind& arg) {
    uint8_t kind;
    if (!read(in, kind)) return false;
    arg = kind;
    return true;
}

template <class Device>
inline int write(Device& out, const any_kind& arg) {
    return write(out, arg.kind());
}

}  // namespace line_kind

/**
 * N-dimensional line in parametric form, that can be a line (by default), a ray, or a segment.
 */
template <class T, int N = 2, class Kind = line_kind::static_kind<line_kind::free>>
struct line : public Kind {
    point<T, N> start;
    point<T, N> vector;

    explicit constexpr line() = default;

    template <class T1, class T2>
    explicit constexpr line(const point<T1, N>& a, const point<T2, N>& b, bool vector = false)
        : start(a), vector(vector ? point<T, N>(a) : point<T, N>(b - a)) {}

    template <class T1, class T2>
    explicit constexpr line(const point<T1, N>& a, const point<T2, N>& b, uint8_t kind,
                            bool vector = false)
        : Kind(kind), start(a), vector(vector ? point<T, N>(a) : point<T, N>(b - a)) {}

    template <class T1, class K1>
    explicit constexpr line(const line<T1, N, K1>& rhs) { (*this) = rhs; }

    constexpr operator bool() const { return vector; }

    void swap(line& rhs) {
        using std::swap;
        swap(start, rhs.start);
        swap(vector, rhs.vector);
        swap(static_cast<Kind&>(*this), rhs);
    }

    template <class T1, class K1>
    constexpr line& operator = (const line<T1, N, K1>& rhs) {
        start = rhs.start;
        vector = rhs.vector;
        static_cast<Kind&>(*this) = rhs.kind();
        return *this;
    }

    constexpr point<T, N> end() const { return start + vector; }

    constexpr uint8_t start_kind() const { return endpoint::start(this->kind()); }
    constexpr uint8_t end_kind() const { return endpoint::end(this->kind()); }

    template <class T1>
    constexpr auto operator()(const T1& t) const {
        return start + t * vector;
    }
};

template <class T, int N = 2>
using ray = line<T, N, line_kind::static_kind<line_kind::closed_ray>>;

template <class T, int N = 2>
using segment = line<T, N, line_kind::static_kind<line_kind::closed_segment>>;

template <class T, int N = 2>
using any_line = line<T, N, line_kind::any_kind>;

template <int N, class T, class K>
struct geometry_traits<line<T, N, K>> : geometry_traits_base<line_tag, point<T, N>> {};

template <int N, class T0, class T1>
inline constexpr auto make_line(const point<T0, N>& a, const point<T1, N>& b, bool vector = false) {
    return line<geometry::scalar_t<T0, T1>, N>(a, b, vector);
}

template <int N, class T0, class T1>
inline constexpr auto make_ray(const point<T0, N>& a, const point<T1, N>& b, bool vector = false) {
    return ray<geometry::scalar_t<T0, T1>, N>(a, b, vector);
}

template <int N, class T0, class T1>
inline constexpr auto make_segment(const point<T0, N>& a, const point<T1, N>& b,
                                   bool vector = false) {
    return segment<geometry::scalar_t<T0, T1>, N>(a, b, vector);
}

template <int N, class T0, class T1>
inline constexpr auto make_any_line(const point<T0, N>& a, const point<T1, N>& b,
                                    uint8_t kind = line_kind::free, bool vector = false) {
    return any_line<geometry::scalar_t<T0, T1>, N>(a, b, kind, vector);
}

template <int N, class T0, class T1, class Line = any_line<geometry::scalar_t<T0, T1>, N>>
inline constexpr Line make_any_line(const point<T0, N>& a, uint8_t akind,
                                    const point<T1, N>& b, uint8_t bkind) {
    if (akind < bkind) return make_any_line(b, bkind, a, akind);
    return Line(a, b, endpoint::combine(akind, bkind));
}

template <int N, class T, class K>
inline void swap(line<T, N, K>& lhs, line<T, N, K>& rhs) { lhs.swap(rhs); }

template <class Device, int N, class T, class K>
inline bool read(Device& in, line<T, N, K>& arg) {
    return read(in, arg.start, arg.vector, static_cast<K&>(arg));
}

template <class Device, int N, class T, class K>
inline int write(Device& out, const line<T, N, K>& arg) {
    return write(out, arg.start, arg.vector, static_cast<const K&>(arg));
}

}  // namespace ac
