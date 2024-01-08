/***************************************************************************************************
 * Copyright 2020 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at
 *http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#include <actl/io/text/decorator/all.hpp>
#include <actl/io/text/decorator/indented.hpp>
#include <actl/std/all.hpp>
#include <actl/test.hpp>

using namespace ac::io;

TEST("json")
{
    using Map = std::map<std::string, std::vector<int>>;
    Map map = {{"aba", {2, 22}}, {"caba", {42, 4, 2}}};
    writeln(cout, text{}, map);
    writeln(cout, std::tuple<spaced<>, text>{}, map);
    std::tuple<spaced<>, pretty, text> spt{};
    // std::get<0>(spt).space = ", ";
    std::get<1>(spt).key_value_separator = ": ";
    writeln(cout, spt, map);
    writeln(cout, spt, std::vector{42, 4, 2});
    writeln(cout, spt, "aba");
    std::tuple<pretty, spaced<>, text> pst{};
    writeln(cout, pst, std::vector{42, 4, 2});
    writeln(cout, pst, "aba");
    // std::tuple<indented<>, spaced<>, pretty, text> ispt{};
    // // write(output_device, spaced{", ", ": "} | pretty{} | text{}, v);
    // std::get<0>(ispt).space = ", ";
    // std::get<1>(ispt).key_value_separator = ": ";
    // std::get<2>(ispt).count = 2;
    // writeln(cout, ispt, map);
}
