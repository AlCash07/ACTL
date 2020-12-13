// Copyright 2018 Roman Rizvanov.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <actl/functional/scalar/all.hpp>
#include <actl/std/map.hpp>
#include <actl/string/split.hpp>
#include <actl/string/trim.hpp>
#include <actl/test.hpp>
#include <actl/types.hpp>
#include <actl/util/stopwatch.hpp>
#include <actl/util/type_traits.hpp>
#include <regex>

namespace ac::tests {

io::formatted<io::file<io::out>> cerr{stderr};

namespace detail {

static auto repeat_key = "repeat"sv;
static auto time_limit_key = "time_limit"sv;

std::vector<std::string> split_va_args(std::string_view va_args_str) {
    return split(va_args_str, [depth = int{}, quote = bool{}, last = char{}](char c) mutable {
        std::swap(last, c);
        switch (last) {
            case ',': {
                return depth == 0 && !quote;
            }
            case '"': {
                if (!quote || c != '\\') {
                    quote = !quote;
                }
                return false;
            }
            case '(': {
                depth += !quote;
                return false;
            }
            case ')': {
                depth -= !quote;
                return false;
            }
        }
        return false;
    });
}

bool test_base::run() {
    ac::stopwatch stopwatch;
    try {
        default_random random{args()};
        auto args = split_va_args(this->args());
        for (auto& arg : args) {
            arg = trim(arg);
        }
        auto name = args.empty() ? "" : args[0];
        if (name.size() > 2 && name.front() == '"' && name.back() == '"')
            name = name.substr(1, name.size() - 2);
        std::map<std::string, std::pair<std::string, std::string>> params;
        for (size_t i = 1; i < args.size(); ++i) {
            auto key_value = split(args[i], '=');
            if (key_value.size() != 2) continue;
            params[trim(key_value[0])] = {trim(key_value[1]), args[i]};
        }
        io::write(cerr, name, ": ");
        auto try_get_param = [&params, this](std::string_view key, auto& value) -> bool {
            auto it = params.find(key.data());
            if (it == params.end()) return false;
            if (!io::read(io::string<io::in>{it->second.first}, io::text_static<>{}, value)) {
                throw "param " + to_string(key) + " has invalid value " +
                    to_string(it->second.first) + "; line = " + to_string(line_number());
            }
            params.erase(it);
            return true;
        };
        int repeat = 1;
        double time_limit = 60;
        try_get_param(repeat_key, repeat);
        try_get_param(time_limit_key, time_limit);
        if (!params.empty()) {
            std::string unknown_params;
            for (auto& key_value : params) {
                if (!unknown_params.empty()) unknown_params += ", ";
                unknown_params += key_value.second.second;
            }
            throw "unknown param" + ((params.size() > 1 ? "s: " : ": ") + unknown_params) +
                "; line = " + to_string(line_number());
        }
        stopwatch.start();
        for (int k = 0; k < repeat; ++k) {
            body(random);
        }
        double elapsed_seconds = stopwatch.seconds();
        if (elapsed_seconds > time_limit) {
            throw "time limit exceeded; line = " + to_string(line_number());
        }
        io::writeln(cerr, "passed [ET = ", elapsed_seconds, "s]");
        return true;
    } catch (const std::string& fail_message) {
        double elapsed_seconds = stopwatch.seconds();
        io::writeln(cerr, "failed: \n", fail_message, "[ET = ", elapsed_seconds, "s]");
        return false;
    }
}

std::vector<test_base*>& all_tests() {
    static std::vector<test_base*> instance;
    return instance;
}

}  // namespace detail

int run(int argc, const char* argv[]) {
    io::write(cerr, io::setprecision{3});
    using namespace detail;
    std::function<bool(const std::string&)> filter = [](const std::string&) -> bool {
        return true;
    };
    if (argc > 1) {
        std::string regex_str;
        for (int i = 1; i < argc; ++i) {
            if (!regex_str.empty()) regex_str += "|";
            regex_str += "(";
            regex_str += argv[i];
            regex_str += ")";
        }
        filter = [regex = std::regex(regex_str)](const std::string& s) {
            return std::regex_search(s, regex);
        };
    }
    std::map<std::string_view, std::vector<test_base*>> tests_per_file;
    if (all_tests().empty()) {
        io::writeln(cerr, "There are no tests :(");
        return 0;
    }
    int total = 0;
    for (auto ptr : all_tests()) {
        if (filter(std::string{ptr->filename()} + " " + std::string{ptr->args()})) {
            tests_per_file[ptr->filename()].push_back(ptr);
            ++total;
        }
    }
    if (tests_per_file.empty()) {
        io::write(cerr, "warning: ignoring command line arguments ");
        for (int i = 1; i < argc; ++i) {
            if (i > 1) io::write(cerr, ", ");
            io::write(cerr, to_string(argv[i]));
        }
        io::write(cerr, ".\n\n", io::flush);
        return run(1, argv);
    }
    auto common_prefix = tests_per_file.begin()->first;
    size_t common_prefix_size = common_prefix.size();
    for (auto file_and_tests : tests_per_file) {
        const auto& filename = file_and_tests.first;
        math::min(inplace(common_prefix_size), filename.size());
        for (size_t i = 0; i < common_prefix_size; ++i) {
            if (filename[i] != common_prefix[i]) {
                common_prefix_size = i;
                break;
            }
        }
    }
    int failed = 0;
    for (auto file_and_tests : tests_per_file) {
        auto filename = file_and_tests.first.substr(common_prefix_size);
        const auto& tests = file_and_tests.second;
        if (!filename.empty()) {
            io::writeln(cerr, "--- ", filename, " ---");
        }
        for (auto test_ptr : tests) {
            failed += static_cast<int>(!test_ptr->run());
        }
        io::write(cerr, '\n', io::flush);
    }
    if (failed == 0) {
        io::writeln(cerr, "Passed all ", total, " tests.");
    } else {
        io::writeln(cerr, "Failed ", failed, " of ", total, " tests.");
    }
    return failed;
}

}  // namespace ac::tests
