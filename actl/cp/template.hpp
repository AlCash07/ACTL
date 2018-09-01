#define random  stdlib_random
#include <unordered_map>
#include <unordered_set>
#include <stdexcept>
#include <algorithm>
#include <iostream>
#include <cassert>
#include <sstream>
#include <vector>
#include <random>
#include <array>
#include <map>
#include <set>
#undef random

#define UN(v) sort(all(v)), (v).erase(unique(all(v)), (v).end())
#define FOR(i, a, b) for (decltype(a) i(a), _B_##i(b); i < _B_##i; ++i)
#define CL(a, b) memset(a, b, sizeof a)
#define all(a) (a).begin(), (a).end()
#define REP(i, n) FOR(i, 0, n)
#define sz(a) int((a).size())
#define ll int64_t
#define ull uint64_t
#define pb push_back
#define Y second
#define X first

// using namespace std;
