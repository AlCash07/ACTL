#include <actl/io/device/memory.hpp>
#include <actl/io/text/arithmetic/integer_read.hpp>
#include <actl/io/text/arithmetic/integer_write.hpp>
#include <actl/io/text/manip.hpp>
#include <actl/test.hpp>
#include <benchmark/benchmark.h>
#include <iostream>

using namespace ac::io;

#if 1
template <class Device, class Format, class Int>
inline ac::index serialize2(Device& od, Format& fmt, Int x)
{
    char s[11];
    auto first = std::end(s);
    do
    {
        auto digit = x % 10;
        x /= 10;
        *--first = static_cast<char>('0' + digit);
    }
    while (x != 0);
    return od.write({first, std::end(s)});
}

static void BM_WriteIntHardcode(benchmark::State& state)
{
    char buf[128] = {};
    text_static<> f;
    for (auto _ : state)
    {
        memory<out> od{buf};
        serialize2(od, f, std::numeric_limits<int>::max());
    }
}

static void BM_WriteIntFull(benchmark::State& state)
{
    char buf[128] = {};
    text_static<> f;
    for (auto _ : state)
    {
        memory<out> od{buf};
        serialize(od, f, std::numeric_limits<int>::max(), text_tag{});
    }
}

BENCHMARK(BM_WriteIntHardcode);
BENCHMARK(BM_WriteIntFull);
BENCHMARK(BM_WriteIntFull);
BENCHMARK(BM_WriteIntHardcode);
#endif

#if 0
template <class Device, class Format, class Int>
inline bool deserialize2(Device& od, Format& fmt, Int& x) {
    Int v = od.get() - '0';
    while (true) {
        Int d = od.peek() - '0';
        if (static_cast<unsigned int>(d) >= 10) break;
        od.move(1);
        v = v * 10 + d;
    }
    x = v;
    return true;
}

static void BM_ReadIntHardcode(benchmark::State& state) {
    char buf[11] = "123456789";
    text_static<> f;
    int x;
    for (auto _ : state) {
        memory<in> id{buf};
        deserialize2(id, f, x);
    }
}

static void BM_ReadIntFull(benchmark::State& state) {
    char buf[11] = "123456789";
    text_static<> f;
    int x;
    for (auto _ : state) {
        memory<in> id{buf};
        read(id, f, x);
    }
}

BENCHMARK(BM_ReadIntHardcode);
BENCHMARK(BM_ReadIntFull);
BENCHMARK(BM_ReadIntFull);
BENCHMARK(BM_ReadIntHardcode);
#endif

BENCHMARK_MAIN();
