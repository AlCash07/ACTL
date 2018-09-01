#pragma once

template <class X>
class poly {
public:
    constexpr poly() : a_({X()}) {}

    constexpr poly(initializer_list<X> list) : a_(list) {
        if (a_.empty()) {
            a_.emplace_back();
        }
    }

    int degree() const { return zeros_ + (int)a_.size() - 1; }
    
    static poly x() { return x_pow(1); }

    static poly x_pow(int n) { return {{Ring(1)}, n}; }

    X operator[](int k) const {
        return 0 <= (k -= zeros_) && k < (int)a.size() ? a_[k] : X();
    }

private:
    std::vector<X> a_;
    int zeros_ = 0;

    void normalize() {
        while (!a_.empty() && a_.back() == X())
            a_.pop_back();
        if (a_.empty()) {
            a_.emplace_back();
            zeros_ = 0;
        }
    }
};

template <class X>
inline poly<X> operator * (const poly<X>& x, const poly<X>& y) {
    poly<X> r { }

    REP (i, sz(a)) REP (j, sz(b))
        r[i + j] = add(r[i + j], mul(a[i], b[j]));
    return r;
}