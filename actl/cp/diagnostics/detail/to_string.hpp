#pragma once

#include <sstream>
#include <cp/type_traits.hpp>

namespace cp { namespace diagnostics { namespace detail {

template <class T>
inline std::string to_string(const T& value);

template <class T, type_kind K = type_kind_of<T>>
struct to_string_impl : to_string_impl<T, general_type_kind_v<K>> {};

template <class T, class = decltype((std::stringstream&)(std::declval<std::stringstream>()) << std::declval<T>())>
std::true_type can_be_stringified_impl(int);

template <class T>
std::false_type can_be_stringified_impl(...);

template <class T>
static constexpr bool can_be_stringified_v = decltype(can_be_stringified_impl<T>(0))::value;

template <class T>
inline std::enable_if_t<can_be_stringified_v<T>, std::string> to_string_impl_impl(const T& value) {
    std::stringstream ss;
    ss << value;
    return ss.str();
}

template <class T>
inline std::enable_if_t<!can_be_stringified_v<T>, std::string> to_string_impl_impl(const T& value) {
    return "<unknown-type>";
}

template <class T>
struct to_string_impl<T, type_kind::unknown> {
    static std::string doit(const T& value) { return to_string_impl_impl(value); }
};

template <class T>
struct to_string_impl<T, type_kind::boolean> {
    static std::string doit(const T& value) { return value ? "true" : "false"; }
};

template <class T>
struct to_string_impl<T, type_kind::string> {
    static std::string doit(const T& value) {
        std::string res(1, '"');
        for (char c : std::string(value)) {
            switch (c) {
                case '\"': res += "\\\""; break;
                case '\n': res += "\\n" ; break;
                case '\r': res += "\\r" ; break;
                default  : res += c;
            }
        }
        return res += '"';
    }
};

template <class T>
struct to_string_impl<T, type_kind::sequence_container> {
    static std::string doit(const T& value) {
        std::string res = "[";
        bool first = true;
        for (const auto& v : value) {
            if (first) first = false;
            else res += ", ";
            res += to_string(v);
        }
        return res += "]";
    }
};

template <int N>
struct to_string_impl<char[N], type_kind::sequence_container> : to_string_impl<char[N], type_kind::string>
{
};

template <class T>
struct to_string_impl<T, type_kind::map> {
    static std::string doit(const T& value) {
        std::string res = "{";
        bool first = true;
        for (auto& kv : value) {
            if (first) first = false;
            else res += ", ";
            res += to_string(kv.first);
            res += ": ";
            res += to_string(kv.second);
        }
        return res += "}";
    }
};

template <class T>
struct to_string_impl<T, type_kind::set> {
    static std::string doit(const T& value) {
        std::string res = "{";
        bool first = true;
        for (auto& v : value) {
            if (first) first = false;
            else res += ", ";
            res += to_string(v);
        }
        return res += "}";
    }
};

template <class T>
struct to_string_impl<T, type_kind::pair> {
    static std::string doit(const T& value) {
        return "(" + to_string(value.first) + ", " + to_string(value.second) + ")";
    }
};

template <class... Ts>
struct to_string_impl<std::tuple<Ts...>, type_kind::tuple> {
    template <int... Indices>
    static std::string doit(const std::tuple<Ts...>& value, std::integer_sequence<int, Indices...>) {
        std::string tupleStr[] = {to_string(std::get<Indices>(value))...};
        std::string res(1, '(');
        for (int i = 0; i < sizeof...(Indices); ++i) {
            if (i) res += ", ";
            res += tupleStr[i];
        }
        return res += ")";
    }

    static std::string doit(const std::tuple<Ts...>& value) {
        return doit(value, std::make_integer_sequence<int, sizeof...(Ts)>());
    }
};

template <>
struct to_string_impl<std::nullptr_t, type_kind::unknown> {
    static std::string doit(std::nullptr_t) {
        return "null";
    }
};

template <class T>
inline std::string to_string(const T& value) {
    return detail::to_string_impl<T, type_kind_of<T>>::doit(value);
}

}}}  // namespace cp::diagnostics::detail
