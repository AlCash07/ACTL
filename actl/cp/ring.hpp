#pragma once

#include <type_traits>
#include <iostream>
#include <cstdint>

namespace cp {

template <class Z, bool Static = false>
struct quotient_ring_base {
    using value_type = Z;
    static constexpr bool is_static = Static;
    static constexpr Z one() { return Z(1); }
};

namespace detail {
    
template <class Z, Z Mod>
struct static_quotient_ring_impl : quotient_ring_base<Z, true> {
    constexpr static_quotient_ring_impl() = default;
    constexpr static_quotient_ring_impl(const static_quotient_ring_impl&) = default;
    static constexpr Z mod() { return Mod; }
};

}  // namespace detail

template <uint64_t Mod, class Z = std::conditional_t<(Mod > (1ull << 32)), uint64_t, uint32_t>>
using static_quotient_ring = detail::static_quotient_ring_impl<Z, (Z)Mod>;

template <class Z>
struct singleton_quotient_ring : quotient_ring_base<Z> {
    static_assert(!std::is_signed<Z>::value, "signed integers are not supported");
    static Z& mod() { static Z mod; return mod; }
};

template <class Z, class R>
inline constexpr void mod_add(Z& x, const Z& y, const R& ring) {
    if (std::is_integral<Z>::value) {
        Z z = ring.mod() - y;
        x < z ? x += y : x -= z;
    } else {
        (x += y) %= ring.mod();
    }
}

template <class Z, class R>
inline constexpr void mod_sub(Z& x, const Z& y, const R& ring) {
    if (std::is_integral<Z>::value) {
        x < y ? x += ring.mod() - y : x -= y;
    } else {
        (x -= y) %= ring.mod();
    }
}

template <class Z, class R>
inline constexpr Z mod_mul_binary(Z x, Z y, const R& ring) {
    for (Z r = Z(); ; ) {
        if (y & 1) mod_add(r, x, ring);
        if (y /= 2) mod_add(x, x, ring);
        else return r;
    }
}

template <class Z, class R>
inline constexpr Z mod_mul(const Z& x, const Z& y, const R& ring) {
    if (!std::is_integral<Z>::value) {
        return (x * y) % ring.mod();
    } else if (R::is_static && ring.mod() == 0) {
        return x * y;
    } else if (sizeof(Z) < sizeof(uint64_t)) {
        return (uint64_t)x * y % ring.mod();
    } else {
        return mod_mul_binary(x, y, ring);
    }
}

template <class Z, class R>
inline constexpr Z mod_div(const Z& x, const Z& y, const R& ring) {
    Z p = ring.mod() - y, a = ring.mod() - ring.one(), q = y, b = ring.one();
    for (; q != Z(); ) {
        mod_sub(a, mod_mul(p / q, b, ring), ring);
        std::swap(a, b);
        std::swap(p %= q, q);
    }
    if (x % p) throw std::exception();
    return mod_mul(a, x / p, ring);
}

template <class Ring>
struct ring_element : Ring {
    using value_type = typename Ring::value_type;
    
    value_type value;

    const Ring& ring() const { return static_cast<const Ring&>(*this); }

    constexpr ring_element() : value(value_type()) {}

    operator value_type() const { return value; }

    template <class... Args>
    constexpr ring_element(const value_type& value, Args&&... args) 
        : Ring(std::forward<Args>(args)...)
        , value(value)
    {}

    constexpr ring_element& operator += (const ring_element& other) {
        mod_add(value, other.value, ring());
        return *this;
    }
    
    constexpr ring_element& operator -= (const ring_element& other) {
        mod_sub(value, other.value, ring());
        return *this;
    }

    constexpr ring_element& operator *= (const ring_element& other) {
        value = mod_mul(value, other.value, ring());
        return *this;
    }    

    constexpr ring_element& operator /= (const ring_element& other) {
        value = mod_div(value, other.value, ring());
        return *this;
    }

    constexpr ring_element operator - (const ring_element& other) const {
        auto r = *this;
        return r -= other;
    }

    constexpr ring_element operator + (const ring_element& other) const {
        auto r = *this;
        return r += other;
    }

    constexpr ring_element operator - () const {
        auto r = value_type();
        mod_sub(r, value, ring());
        return ring_element(r, ring());
    }

    constexpr ring_element operator * (ring_element other) const {
        return ring_element(mod_mul(value, other.value, ring()), ring());
    }

    constexpr ring_element operator / (ring_element other) const {
        return ring_element(mod_div(value, other.value, ring()), ring());
    }

    constexpr bool operator == (const ring_element& other) const {
        return value == other.value;
    }

    constexpr bool operator != (const ring_element& other) const {
        return value != other.value;
    }

    friend std::istream& operator >> (std::istream& in, ring_element& x) {
        return in >> x.value;
    }

    friend std::ostream& operator << (std::ostream& out, const ring_element& x) {
        return out << x.value;
    }

    void check() const {
        assert(0 <= value && value < ring().mod());
    }
};

template <uint64_t Mod>
using int_modulo = ring_element<static_quotient_ring<Mod>>;

}  // namespace cp
