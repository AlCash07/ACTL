#pragma once

#include <type_traits>
#include <random>

namespace cp {

template <class Rng>
struct rng_decorator : public Rng {
    using result_type = typename Rng::result_type;

    constexpr rng_decorator() = default;

    template <class Seed>
    rng_decorator(Seed&& s) { seed(std::forward<Seed>(s)); }
    
    /**
     * Generates uniform random number in the range [from, to).
     */
    template <class T>
    T uniform(T from, T to) { return uniform_impl(from, to, std::is_integral<T>()); }

    /**
     * Generates uniform random number in the range [0, to).
     */
    template <class T>
    T uniform(T to) { return uniform(T(), to); }

    /**
     * Generates normal (Gaussian) random number with mean = @p mean and stddev = @p stddev.
     */
    template <class T = double>
    T normal(T mean = T(), T stddev = T(1)) { return std::normal_distribution<T>(mean, stddev)(base()); }

    void seed(const char* value) {
        Rng rng0;
        do {
            rng0();
            base() = rng0;
            for (auto i = value; *i; ++i) {
                this->state[0] += (result_type)*i;
            }
        } while (base()() == base()());  // improbable unless RNG state is degenerate
    }

private:
    template <class T>
    T uniform_impl(T from, T to, std::true_type) { return std::uniform_int_distribution<T>(from, to - 1)(*this); }

    template <class T>
    T uniform_impl(T from, T to, std::false_type) { return std::uniform_real_distribution<T>(from, to)(*this); }

    Rng& base() { return static_cast<Rng&>(*this); }
};

}  // namespace cp