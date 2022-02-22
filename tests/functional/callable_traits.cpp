// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#include <actl/functional/callable_traits.hpp>
#include <functional>
#include "test.hpp"

namespace {

template <class Function, bool IsNoexcept>
void test_free_function_traits()
{
    static_assert(0ul == ac::arity_v<Function>);
    static_assert(std::is_same_v<void, ac::return_type_t<Function>>);
    static_assert(!ac::is_member_function_v<Function>);
    static_assert(IsNoexcept == ac::is_noexcept_v<Function>);
}

} // namespace

TEST_CASE("void 0 arity free function: value, pointer, reference")
{
    test_free_function_traits<void(), false>();
    test_free_function_traits<void() noexcept, true>();
    test_free_function_traits<void (*)(), false>();
    test_free_function_traits<void (*)() noexcept, true>();
    test_free_function_traits<void (&)(), false>();
    test_free_function_traits<void (&)() noexcept, true>();
}

using free_function_params =
    int && (int, const int, int&, const int&, int*, const int*) noexcept;
static_assert(6ul == ac::arity_v<free_function_params>);
static_assert(std::is_same_v<int&&, ac::return_type_t<free_function_params>>);
static_assert(
    std::is_same_v<int, ac::parameter_type_t<0, free_function_params>>);
static_assert(
    std::is_same_v<int, ac::parameter_type_t<1, free_function_params>>);
static_assert(
    std::is_same_v<int&, ac::parameter_type_t<2, free_function_params>>);
static_assert(
    std::is_same_v<const int&, ac::parameter_type_t<3, free_function_params>>);
static_assert(
    std::is_same_v<int*, ac::parameter_type_t<4, free_function_params>>);
static_assert(
    std::is_same_v<const int*, ac::parameter_type_t<5, free_function_params>>);
static_assert(!ac::is_member_function_v<free_function_params>);
static_assert(ac::is_noexcept_v<free_function_params>);

namespace {

struct S
{
    int member_function();
    int member_function_noexcept() noexcept;
    int const_member_function() const;
    int const_member_function_noexcept() const noexcept;

    int reference_member_function() &;
    int reference_member_function_noexcept() & noexcept;
    int const_reference_member_function() const&;
    int const_reference_member_function_noexcept() const& noexcept;

    int rvalue_reference_member_function() &&;
    int rvalue_reference_member_function_noexcept() && noexcept;
    int const_rvalue_reference_member_function() const&&;
    int const_rvalue_reference_member_function_noexcept() const&& noexcept;

    const int& member_function_params(const int, int&&) const noexcept;
};

template <class MemberF, class ClassParam, bool IsNoexcept>
void test_member_function_traits()
{
    static_assert(1ul == ac::arity_v<MemberF>);
    static_assert(std::is_same_v<int, ac::return_type_t<MemberF>>);
    static_assert(std::is_same_v<ClassParam, ac::parameter_type_t<0, MemberF>>);
    static_assert(ac::is_member_function_v<MemberF>);
    static_assert(IsNoexcept == ac::is_noexcept_v<MemberF>);
}

} // namespace

TEST_CASE("member function pointers")
{
    test_member_function_traits<decltype(&S::member_function), S&, false>();
    test_member_function_traits<
        decltype(&S::member_function_noexcept),
        S&,
        true>();
    test_member_function_traits<
        decltype(&S::const_member_function),
        const S&,
        false>();
    test_member_function_traits<
        decltype(&S::const_member_function_noexcept),
        const S&,
        true>();
    test_member_function_traits<
        decltype(&S::reference_member_function),
        S&,
        false>();
    test_member_function_traits<
        decltype(&S::reference_member_function_noexcept),
        S&,
        true>();
    test_member_function_traits<
        decltype(&S::const_reference_member_function),
        const S&,
        false>();
    test_member_function_traits<
        decltype(&S::const_reference_member_function_noexcept),
        const S&,
        true>();
    test_member_function_traits<
        decltype(&S::rvalue_reference_member_function),
        S&&,
        false>();
    test_member_function_traits<
        decltype(&S::rvalue_reference_member_function_noexcept),
        S&&,
        true>();
    test_member_function_traits<
        decltype(&S::const_rvalue_reference_member_function),
        const S&&,
        false>();
    test_member_function_traits<
        decltype(&S::const_rvalue_reference_member_function_noexcept),
        const S&&,
        true>();
}

using mem_fn_params = decltype(&S::member_function_params);
static_assert(3ul == ac::arity_v<mem_fn_params>);
static_assert(std::is_same_v<const int&, ac::return_type_t<mem_fn_params>>);
static_assert(std::is_same_v<const S&, ac::parameter_type_t<0, mem_fn_params>>);
static_assert(std::is_same_v<int, ac::parameter_type_t<1, mem_fn_params>>);
static_assert(std::is_same_v<int&&, ac::parameter_type_t<2, mem_fn_params>>);
static_assert(ac::is_member_function_v<mem_fn_params>);
static_assert(ac::is_noexcept_v<mem_fn_params>);

namespace {

struct fn_object
{
    S operator()();
};

struct fn_object_noexcept
{
    S operator()() noexcept;
};

struct const_fn_object
{
    S operator()() const;
};

struct const_fn_object_noexcept
{
    S operator()() const noexcept;
};

struct fn_object_params
{
    const void* operator()(const S*, int&&, S) const noexcept;
};

} // namespace

static_assert(0ul == ac::arity_v<fn_object>);
static_assert(0ul == ac::arity_v<fn_object&>);
static_assert(0ul == ac::arity_v<fn_object&&>);
static_assert(std::is_same_v<S, ac::return_type_t<fn_object>>);
static_assert(!ac::is_member_function_v<fn_object>);
static_assert(!ac::is_noexcept_v<fn_object>);

static_assert(0ul == ac::arity_v<fn_object_noexcept>);
static_assert(std::is_same_v<S, ac::return_type_t<fn_object_noexcept>>);
static_assert(!ac::is_member_function_v<fn_object_noexcept>);
static_assert(ac::is_noexcept_v<fn_object_noexcept>);

static_assert(0ul == ac::arity_v<const_fn_object>);
static_assert(0ul == ac::arity_v<const const_fn_object>);
static_assert(0ul == ac::arity_v<const const_fn_object&>);
static_assert(std::is_same_v<S, ac::return_type_t<const const_fn_object>>);
static_assert(!ac::is_member_function_v<const_fn_object>);
static_assert(!ac::is_noexcept_v<const_fn_object>);

static_assert(0ul == ac::arity_v<const_fn_object_noexcept>);
static_assert(std::is_same_v<S, ac::return_type_t<const_fn_object_noexcept>>);
static_assert(!ac::is_member_function_v<const_fn_object_noexcept>);
static_assert(ac::is_noexcept_v<const_fn_object_noexcept>);

static_assert(3ul == ac::arity_v<fn_object_params>);
static_assert(3ul == ac::arity_v<fn_object_params&>);
static_assert(3ul == ac::arity_v<fn_object_params&&>);
static_assert(std::is_same_v<const void*, ac::return_type_t<fn_object_params>>);
static_assert(
    std::is_same_v<const S*, ac::parameter_type_t<0, fn_object_params>>);
static_assert(std::is_same_v<int&&, ac::parameter_type_t<1, fn_object_params>>);
static_assert(std::is_same_v<S, ac::parameter_type_t<2, fn_object_params>>);
static_assert(!ac::is_member_function_v<fn_object_params>);
static_assert(ac::is_noexcept_v<fn_object_params>);

using std_function = std::function<int*(int&)>;
static_assert(1ul == ac::arity_v<std_function>);
static_assert(std::is_same_v<int*, ac::return_type_t<std_function>>);
static_assert(std::is_same_v<int&, ac::parameter_type_t<0, std_function>>);
static_assert(!ac::is_member_function_v<std_function>);
