#include <actl/cp/test.hpp>
#include <actl/cp/lexical_cast.hpp>
#include <actl/cp/string/split.hpp>
#include <actl/cp/string/trim.hpp>
#include <actl/cp/diagnostics/detail/split_va_args.hpp>
#include <actl/cp/utility.hpp>
#include <actl/cp/time.hpp>

#include <iomanip>
#include <regex>
#include <cmath>
#include <map>

namespace cp { namespace tests {

namespace detail {

static const char* repeat_key = "repeat";
static const char* time_limit_key = "time_limit";

bool test_base::run() {
    using diagnostics::detail::to_string;
    cp::stopwatch stopwatch;
    try {
        default_random random(args());
        auto args = diagnostics::detail::split_va_args(this->args());
        for (auto& v : args) {
            v = trim(v);
        }
        auto name = args.empty() ? "" : args[0];
        if (name.size() > 2 && name.front() == '"' && name.back() == '"')
            name = name.substr(1, name.size() - 2);
        std::map<std::string, std::pair<std::string, std::string>> params;
        for (size_t i = 1; i < args.size(); ++i) {
            auto kv = split(args[i], '=');
            if (kv.size() != 2) continue;
            params[trim(kv[0])] = {trim(kv[1]), args[i]};
        }
        std::cerr << name << ": ";
        auto try_get_param = [&params, this](const char* key, auto& value) -> bool {
            auto it = params.find(key);
            if (it == params.end()) return false;
            if (!TRY_ASSIGN(value, lexical_cast<std::decay_t<decltype(value)>>(it->second.first))) {
                throw "param " + to_string(key) + " has invalid value " + to_string(it->second.first) +
                        "; line = " + to_string(line());
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
            for (auto& kv : params) {
                if (!unknown_params.empty()) unknown_params += ", ";
                unknown_params += kv.second.second;
            }
            throw "unknown param" + ((params.size() > 1 ? "s: " : ": ") + unknown_params) + 
                    "; line = " + to_string(line());
        }
        stopwatch.start();
        for (int k = 0; k < repeat; ++k) {
            body(random);
        }
        double elapsed_seconds = stopwatch.seconds();
        if (elapsed_seconds > time_limit) {
            throw "time limit exceeded; line = " + to_string(line());
        }
        std::cerr << "passed [ET = " << std::fixed << std::setprecision(3) << elapsed_seconds << "s]" << std::endl;
        return true;
    } catch (const std::string& fail_message) {
        double elapsed_seconds = stopwatch.seconds();
        std::cerr << "failed: " << fail_message 
                    << " [ET = " << std::fixed << std::setprecision(3) << elapsed_seconds << "s]" << std::endl;
        return false;
    }
}

std::vector<test_base*>& all_tests() {
    static std::vector<test_base*> instance;
    return instance;
}

}  // namespace detail

int run(int argc, const char* argv[]) {
    using namespace detail;
    std::function<bool(const std::string&)> filter = [](const std::string&) -> bool { return true; };
    if (argc > 1) {
        std::string regex_str;
        for (int i = 1; i < argc; ++i) {
            if (!regex_str.empty()) regex_str += "|";
            regex_str += "(";
            regex_str += argv[i];
            regex_str += ")";
        }
        filter = [regex = std::regex(regex_str)](const std::string& s) -> bool {
            return std::regex_search(s, regex);
        };
    }
    std::map<std::string, std::vector<test_base*>> tests_per_file;
    if (all_tests().empty()) {
        std::cerr << "There are no tests :(" << std::endl;
        return 0;
    }
    int total = 0;
    for (auto ptr : all_tests()) {
        if (filter(ptr->filename() + std::string(" ") + ptr->args())) {
            tests_per_file[ptr->filename()].push_back(ptr);
            ++total;
        }
    }
    if (tests_per_file.empty()) {
        std::cerr << "warning: ignoring command line arguments ";
        for (int i = 1; i < argc; ++i) {
            if (i > 1) std::cerr << ", ";
            std::cerr << diagnostics::detail::to_string(argv[i]);
        }
        std::cerr << "." << std::endl << std::endl;
        return run(1, argv);
    }
    std::string common_prefix = tests_per_file.begin()->first;
    int common_prefix_size = (int)common_prefix.size();
    for (auto file_and_tests : tests_per_file) {
        const auto& filename = file_and_tests.first;
        common_prefix_size = std::min(common_prefix_size, (int)filename.size());
        for (int i = 0; i < common_prefix_size; ++i)
            if (filename[i] != common_prefix[i]) {
                common_prefix_size = i;
                break;
            }
    }
    int num_ok = 0;
    for (auto file_and_tests : tests_per_file) {
        auto filename = file_and_tests.first.substr(common_prefix_size);
        const auto& tests = file_and_tests.second;
        if (!filename.empty()) {
            std::cerr << "--- " << filename << " ---" << std::endl;
        }
        for (auto tc_ptr : tests) {
            num_ok += tc_ptr->run();
        }
        std::cerr << std::endl;
    }
    if (num_ok == total) {
        std::cerr << "Passed all tests." << std::endl;
    } else {
        std::cerr << "Failed " << total - num_ok << "/" << total << " tests." << std::endl;
    }
    return 0;
}

}}  // namespace tests::cp
