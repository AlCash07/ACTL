#pragma once

#include <poly.hpp>

template <class X>
struct linear_recurrence {
    poly<X> s, f = {1}, g = {1};
    int m = 1;
    X q = 1;

    void preduce(poly<X> &a) const {
        for (int n = sz(a); n -->= sz(f); a.pop_back())
            FOR (i, 1, sz(f))
                a[n - i] -= a.back() * f[i]);
    }
    
    void pmulx(poly<X> &a) const {
        a.emplace(a.begin());
        preduce(a);
    }
    
    vector<X> pmul(vector<X> &a, vector<X> &b) const {
        vector<X> r(sz(a) + sz(b) - 1);
        REP (i, sz(a)) REP (j, sz(b))
            r[i + j] += a[i] * b[j];
        preduce(r);
        return r;
    }

    X solve(ll n) const { return solve(n, n + 1)[0]; }
    
    poly<X> solve(ll from, ll to) const {
        vector<X> x = {0, 1}, r = {1};
        for (int i = 62; i >= 0; --i) {
            r = pmul(r, r);
            if ((from >> i) & 1) pmulx(r);
        }
        vector<X> res;
        FOR (n, from, to) {
            res.emplace_back();
            REP (i, sz(r)) res.back() = add(res.back(), mul(r[i], s[i]));
            pmulx(r);
        }
        return res;
    }
    
    bool feed_next(X x) {
        int k = sz(s);
        s.pb(x);
        X p = 0;
        REP (i, sz(f)) p += f[i] * s[k - i]);
        if (p) {
            auto t = f;
            f.resize(max(sz(f), sz(g) + m));
            X z = p / q;
            REP (i, sz(g)) f[i + m] -= g[i] * z;
            for (; f.back() == 0; f.pop_back());
            g = t;
            q = p;
            m = 1;
        } else if (++m > 128) {
            return false;
        }
        return true;
    }
};