#pragma once

#include <cp/diagnostics/detail/to_string.hpp>
#include <cp/random.hpp>

#include <iostream>

#define CONCATENATE_IMPL(x, y)  x##y
#define CONCATENATE(x, y)       CONCATENATE_IMPL(x, y)

#define TEST_IMPL(cn, as)                                           \
    namespace {                                                     \
	struct cn : cp::tests::detail::test_base {                      \
        const char* filename() const override { return __FILE__; }  \
        int line() const override { return __LINE__; }              \
        const char* args() const override { return as; }            \
        void body(cp::default_random&) const override;              \
        cn() { cp::tests::detail::all_tests().push_back(this); }    \
        static cn initializer;                                      \
    } cn::initializer; }                                            \
    inline void cn::body(cp::default_random& random) const

#define TEST(...)                   TEST_IMPL(CONCATENATE(_tesT_, __COUNTER__), #__VA_ARGS__)

#define ASSERT_EQUAL                cp::tests::detail::assert_impl(__FILE__, __LINE__).check<true>
#define ASSERT_NOT_EQUAL            cp::tests::detail::assert_impl(__FILE__, __LINE__).check<false>
#define ASSERT_TRUE                 cp::tests::detail::assert_impl(__FILE__, __LINE__).check_true
#define ASSERT_FALSE                cp::tests::detail::assert_impl(__FILE__, __LINE__).check_false
#define ASSERT_THROWS(expression)   cp::tests::detail::assert_throws(__FILE__, __LINE__, [&]{ (void)(expression); })

namespace cp { namespace tests {

namespace detail {

struct assert_impl {
    const char* filename;
    int line;
    assert_impl(const char* filename, int line) : filename(filename), line(line) {}

    template <bool Equal, class T1, class T2>
    inline void check(const T1& expected, const T2& actual) const {
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wsign-compare"
        if ((expected == actual) == Equal) return;
#pragma clang diagnostic pop
        std::stringstream ss;
        if (!Equal) ss << "not ";
        ss << "expected = " << diagnostics::detail::to_string(expected)
           << ", actual = " << diagnostics::detail::to_string(actual);
        ss << "; line = " << line;
        throw ss.str();
    }

    template <bool Equal>
    inline void check(double expected, double actual, double eps) const {
        double num = std::abs(expected - actual);
        double denom = std::max(std::max(std::abs(expected), std::abs(actual)), 1.0);
        if ((num <= eps * denom) == Equal) return;
        std::stringstream ss;
        if (!Equal) ss << "not ";
        ss << "expected = " << expected << ", actual = " << actual << ", error = " << num / denom;
        ss << "; line = " << line;
        throw ss.str();
    }

    inline void check_true(bool condition) const { check<true>(true, condition); }

    inline void check_false(bool condition) const { check<true>(false, condition); }
};

template <class F>
inline void assert_throws(const char* filename, int line, const F& f) {
    bool ok = true;
    try { f(); ok = false; } catch(...) {}
    if (ok) return;
    std::stringstream ss;
    ss << "expected exception";
    ss << "; line = " << line;
    throw ss.str();
}

struct test_base {
    virtual const char* filename() const = 0;
    virtual int line() const = 0;
    virtual const char* args() const = 0;
    virtual void body(default_random& random) const = 0;
    
    bool run();
};

std::vector<test_base*>& all_tests();

}  // namespace detail

int run(int argc, const char* argv[]);

}}  // namespace cp::tests
