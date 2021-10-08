#include <actl/meta/can_list_initialize.hpp>

struct aggregate
{
    int x;
    float y;
};

static_assert(ac::can_list_initialize_v<int, int>);
static_assert(!ac::can_list_initialize_v<int, long long>);
static_assert(ac::can_list_initialize_v<aggregate, int, float>);
static_assert(!ac::can_list_initialize_v<aggregate, int, int>);
static_assert(ac::can_list_initialize_v<int[3], int, int, int>);
static_assert(ac::can_list_initialize_v<int[3], int, int>);
static_assert(!ac::can_list_initialize_v<int[3], int, int, int, int>);
