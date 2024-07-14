// Copyright 2024 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt)

#include <actl/functional/name/all.hpp>
#include <string_view>

namespace ac {

using namespace std::string_view_literals;

constexpr int simple_function() noexcept {
    static_assert(AC_FUNCTION_NAME == "simple_function"sv);
    // Note how inconsistent the results are across compilers.
#if AC_COMPILER_CLANG()
    static_assert(AC_FUNCTION_SIGNATURE == "int ac::simple_function()"sv);
#elif AC_COMPILER_GCC()
    static_assert(
        AC_FUNCTION_SIGNATURE == "constexpr int ac::simple_function()"sv
    );
#elif AC_COMPILER_MSVC()
    static_assert(
        AC_FUNCTION_SIGNATURE ==
        "int __cdecl ac::simple_function(void) noexcept"sv
    );
#endif
    return 0;
}

template<class Result, class Input>
constexpr Result function_template(Input const& input) {
    static_assert(AC_FUNCTION_NAME == "function_template"sv);
    return Result{};
}

[[maybe_unused]] constexpr auto unused = function_template<bool>(0);

} // namespace ac
