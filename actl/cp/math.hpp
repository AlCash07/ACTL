#pragma once

namespace math {

template <class X, class N>
inline std::enable_if_t<std::is_integral<N>::value, X> pow(X x, N n) {
    for (X r(1); ; ) {
        if (n & 1) r *= x;
        if (n /= 2) x *= x;
        else return r;
    }
}

}  // namespace math