// Copyright 2018 Oleksandr Bacherikov.
//
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt).

#include <actl/string/char_classes.hpp>
#include <cctype>
#include "test.hpp"

TEST_CASE("char classes are identical to the ones in <cctype>") {
    for (char c = 0; c != std::numeric_limits<char>::max(); ++c) {
        CHECK((std::isdigit(int{c}) != 0) == ac::is_digit(c));
        CHECK((std::islower(int{c}) != 0) == ac::is_lower(c));
        CHECK((std::isupper(int{c}) != 0) == ac::is_upper(c));
        CHECK((std::isalpha(int{c}) != 0) == ac::is_alpha(c));
        CHECK((std::isalnum(int{c}) != 0) == ac::is_alphanumeric(c));
        CHECK((std::isspace(int{c}) != 0) == ac::is_space(c));
    }
}

/* constexpr */
static_assert(ac::is_digit('2'));
static_assert(!ac::is_digit('b'));
static_assert(ac::is_lower('b'));
static_assert(!ac::is_lower('2'));
static_assert(ac::is_upper('B'));
static_assert(!ac::is_upper('b'));
static_assert(ac::is_alpha('b'));
static_assert(!ac::is_alpha('2'));
static_assert(ac::is_alphanumeric('2'));
static_assert(!ac::is_alphanumeric('\n'));
static_assert(ac::is_space('\n'));
static_assert(!ac::is_space('b'));

/* noexcept */
static_assert(noexcept(ac::is_digit(char{})));
static_assert(noexcept(ac::is_lower(char{})));
static_assert(noexcept(ac::is_upper(char{})));
static_assert(noexcept(ac::is_alpha(char{})));
static_assert(noexcept(ac::is_alphanumeric(char{})));
static_assert(noexcept(ac::is_space(char{})));
