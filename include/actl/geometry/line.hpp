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
    free              = endpoint::combine(endpoint::free,   endpoint::free),
    closed_ray        = endpoint::combine(endpoint::closed, endpoint::free),
    open_ray          = endpoint::combine(endpoint::open,   endpoint::free),
    closed_segment    = endpoint::combine(endpoint::closed, endpoint::closed),
    half_open_segment = endpoint::combine(endpoint::open,   endpoint::closed),
    open_segment      = endpoint::combine(endpoint::open,   endpoint::open)
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

    friend struct ac::io::serialization_access;

    template <class Device, class Format>
    index serialize(Device& od, Format& fmt) const {
        return write(od, fmt, kind_);
    }

    template <class Device, class Format>
    bool deserialize(Device& id, Format& fmt) {
        return read(id, fmt, kind_) && is_valid(kind_);
    }
};

}  // namespace line_kind

/**
 * N-dimensional line in parametric form, that can be a line (by default), a ray, or a segment.
 */
template <class T, index N = 2, class Kind = line_kind::static_kind<line_kind::free>>
class line : public Kind {
public:
    point<T, N> start;
    point<T, N> vector;

    constexpr line() = default;

    template <class T1 = T, class T2 = T>
    explicit constexpr line(const point<T1, N>& a, const point<T2, N>& b, bool vector = false)
        : start{a}, vector{vector ? point<T, N>{a} : point<T, N>{b - a}} {}

    template <class T1 = T, class T2 = T>
    explicit constexpr line(const point<T1, N>& a, const point<T2, N>& b, uint8_t kind,
                            bool vector = false)
        : Kind{kind}, start{a}, vector{vector ? point<T, N>{a} : point<T, N>{b - a}} {}

    template <class T1, class K1>
    explicit constexpr line(const line<T1, N, K1>& rhs)
        : Kind{rhs.kind()}, start{rhs.start}, vector{rhs.vector} {}

    constexpr operator bool() const { return vector; }

    friend void swap(line& lhs, line& rhs) {
        using std::swap;
        swap(lhs.start, rhs.start);
        swap(lhs.vector, rhs.vector);
        swap(static_cast<Kind&>(lhs), rhs);
    }

    constexpr point<T, N> end() const { return start + vector; }

    constexpr uint8_t start_kind() const { return endpoint::start(this->kind()); }
    constexpr uint8_t end_kind() const { return endpoint::end(this->kind()); }

    template <class Policy, class T1>
    constexpr auto operator()(const Policy& policy, const T1& t) const {
        return start + product(policy, t, vector);
    }

    template <class T1>
    constexpr auto operator()(const T1& t) const {
        return (*this)(default_policy, t);
    }

private:
    Kind& base() { return *this; }
    const Kind& base() const { return *this; }

    INTROSPECT(start, vector, base())
};

template <class T, index N = 2>
using ray = line<T, N, line_kind::static_kind<line_kind::closed_ray>>;

template <class T, index N = 2>
using segment = line<T, N, line_kind::static_kind<line_kind::closed_segment>>;

template <class T, index N = 2>
using any_line = line<T, N, line_kind::any_kind>;

template <index N, class T, class K>
struct geometry_traits<line<T, N, K>> : geometry_traits_base<line_tag, point<T, N>> {};

template <index N, class T0, class T1>
inline constexpr auto make_line(const point<T0, N>& a, const point<T1, N>& b, bool vector = false) {
    return line<geometry::scalar_t<T0, T1>, N>{a, b, vector};
}

template <index N, class T0, class T1>
inline constexpr auto make_ray(const point<T0, N>& a, const point<T1, N>& b, bool vector = false) {
    return ray<geometry::scalar_t<T0, T1>, N>{a, b, vector};
}

template <index N, class T0, class T1>
inline constexpr auto make_segment(const point<T0, N>& a, const point<T1, N>& b,
                                   bool vector = false) {
    return segment<geometry::scalar_t<T0, T1>, N>{a, b, vector};
}

template <index N, class T0, class T1>
inline constexpr auto make_any_line(const point<T0, N>& a, const point<T1, N>& b,
                                    uint8_t kind = line_kind::free, bool vector = false) {
    return any_line<geometry::scalar_t<T0, T1>, N>{a, b, kind, vector};
}

template <index N, class T0, class T1, class Line = any_line<geometry::scalar_t<T0, T1>, N>>
inline constexpr Line make_any_line(const point<T0, N>& a, uint8_t akind,
                                    const point<T1, N>& b, uint8_t bkind) {
    if (akind < bkind) return make_any_line(b, bkind, a, akind);
    return Line{a, b, endpoint::combine(akind, bkind)};
}

template <class Policy, index N, class T, class K>
inline constexpr bool degenerate(const Policy& policy, const line<T, N, K>& l) {
    return degenerate(l.vector);
}

// Policy to indicate that scalar is expected instead of a point. This scalar can be passed to line
// operator () to get the point.
template <class Policy>
struct line_scalar_policy : virtual op::policy {
    explicit line_scalar_policy(const Policy& x) : policy{x} {}

    const Policy& policy;
};

}  // namespace ac
