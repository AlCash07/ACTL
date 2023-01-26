// Copyright 2017 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include <actl/geometry/point.hpp>
#include <cstdint>

namespace ac {

enum class endpoint : uint8_t
{
    free = 0,
    closed = 1,
    open = 2
};

enum class line_kind : uint8_t;

inline constexpr uint8_t combine(endpoint begin, endpoint end)
{
    return static_cast<uint8_t>(
        static_cast<int>(begin) | static_cast<int>(end) << 2);
}

inline constexpr endpoint begin(line_kind kind)
{
    return static_cast<endpoint>(static_cast<int>(kind) & 3);
}

inline constexpr endpoint end(line_kind kind)
{
    return static_cast<endpoint>(static_cast<int>(kind) >> 2);
}

inline constexpr bool is_valid(line_kind kind)
{
    int s = static_cast<int>(begin(kind));
    return static_cast<int>(end(kind)) <= s && s < 3;
}

// clang-format off
enum class line_kind : uint8_t {
    free              = combine(endpoint::free,   endpoint::free),
    closed_ray        = combine(endpoint::closed, endpoint::free),
    open_ray          = combine(endpoint::open,   endpoint::free),
    closed_segment    = combine(endpoint::closed, endpoint::closed),
    half_open_segment = combine(endpoint::open,   endpoint::closed),
    open_segment      = combine(endpoint::open,   endpoint::open)
};
// clang-format on

namespace detail {

template <line_kind Kind>
struct static_kind
{
    static_kind(line_kind kind = Kind)
    {
        (*this) = kind;
    }

    static constexpr line_kind kind()
    {
        return Kind;
    }

    void operator=(line_kind kind)
    {
        AC_ASSERT(Kind == kind);
    }
};

class any_kind
{
public:
    any_kind(line_kind kind = line_kind::free)
    {
        (*this) = kind;
    }

    void operator=(line_kind kind)
    {
        AC_ASSERT(is_valid(kind));
        kind_ = kind;
    }

    line_kind kind() const
    {
        return kind_;
    }

private:
    line_kind kind_;

    friend struct ac::io::serialization_access;

    bool write_final(Device auto& od, Format auto& fmt) const
    {
        return write(od, fmt, static_cast<int>(kind_));
    }

    bool read_final(Device auto& id, Format auto& fmt)
    {
        return read(id, fmt, kind_) && is_valid(kind_);
    }
};

} // namespace detail

/// N-dimensional line in parametric form, that can be a line (by default), a
/// ray, or a segment.
template <
    class T,
    index N = 2,
    class Kind = detail::static_kind<line_kind::free>>
class line : public Kind
{
public:
    point<T, N> begin;
    point<T, N> vector;

    constexpr line() = default;

    template <class T1 = T, class T2 = T>
    explicit constexpr line(
        point<T1, N> const& a, point<T2, N> const& b, bool vector = false)
        : begin{a}, vector{vector ? point<T, N>{b} : point<T, N>{b - a}}
    {}

    template <class T1 = T, class T2 = T>
    explicit constexpr line(
        point<T1, N> const& a,
        point<T2, N> const& b,
        line_kind kind,
        bool vector = false)
        : Kind{kind}
        , begin{a}
        , vector{vector ? point<T, N>{b} : point<T, N>{b - a}}
    {}

    template <class T1, class K1>
    explicit constexpr line(line<T1, N, K1> const& rhs)
        : Kind{rhs.kind()}, begin{rhs.begin}, vector{rhs.vector}
    {}

    template <class T1, class K1>
    constexpr line& operator=(line<T1, N, K1> const& rhs)
    {
        Kind::operator=(rhs.kind());
        begin = rhs.begin;
        vector = rhs.vector;
        return *this;
    }

    friend void swap(line& lhs, line& rhs)
    {
        using std::swap;
        swap(lhs.begin, rhs.begin);
        swap(lhs.vector, rhs.vector);
        swap(static_cast<Kind&>(lhs), rhs);
    }

    constexpr point<T, N> end() const
    {
        return begin + vector;
    }

    template <class Policy, class T1>
    constexpr auto operator()(Policy const& policy, T1 const& t) const
    {
        return begin + product(policy, t, vector);
    }

    template <class T1>
    constexpr auto operator()(T1 const& t) const
    {
        return (*this)(default_policy, t);
    }

private:
    Kind& base()
    {
        return *this;
    }

    Kind const& base() const
    {
        return *this;
    }

    INTROSPECT(begin, vector, base())
};

template <class T, index N = 2>
using ray = line<T, N, detail::static_kind<line_kind::closed_ray>>;

template <class T, index N = 2>
using segment = line<T, N, detail::static_kind<line_kind::closed_segment>>;

template <class T, index N = 2>
using any_line = line<T, N, detail::any_kind>;

template <index N, class T, class K>
struct geometry_traits<line<T, N, K>>
    : geometry_traits_base<line_tag, point<T, N>>
{};

template <index N, class T0, class T1>
constexpr auto make_line(
    point<T0, N> const& a, point<T1, N> const& b, bool vector = false)
{
    return line<geometry::scalar_t<T0, T1>, N>{a, b, vector};
}

template <index N, class T0, class T1>
constexpr auto make_ray(
    point<T0, N> const& a, point<T1, N> const& b, bool vector = false)
{
    return ray<geometry::scalar_t<T0, T1>, N>{a, b, vector};
}

template <index N, class T0, class T1>
constexpr auto make_segment(
    point<T0, N> const& a, point<T1, N> const& b, bool vector = false)
{
    return segment<geometry::scalar_t<T0, T1>, N>{a, b, vector};
}

template <index N, class T0, class T1>
constexpr auto make_any_line(
    point<T0, N> const& a,
    point<T1, N> const& b,
    line_kind kind = line_kind::free,
    bool vector = false)
{
    return any_line<geometry::scalar_t<T0, T1>, N>{a, b, kind, vector};
}

template <
    index N,
    class T0,
    class T1,
    class Line = any_line<geometry::scalar_t<T0, T1>, N>>
constexpr Line make_any_line(
    point<T0, N> const& a,
    endpoint akind,
    point<T1, N> const& b,
    endpoint bkind)
{
    if (akind < bkind)
        return make_any_line(b, bkind, a, akind);
    return Line{a, b, static_cast<line_kind>(combine(akind, bkind))};
}

template <class Policy, index N, class T, class K>
constexpr bool degenerate(Policy const& policy, line<T, N, K> const& l)
{
    return degenerate(policy, l.vector);
}

// Policy to indicate that scalar is expected instead of a point. This scalar
// can be passed to line operator () to get the point.
template <class Policy>
struct line_scalar_policy : virtual policy
{
    explicit line_scalar_policy(Policy const& x) : policy{x} {}

    Policy const& policy;
};

} // namespace ac
