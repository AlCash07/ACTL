/***************************************************************************************************
 * Copyright 2018 Oleksandr Bacherikov.
 *
 *             Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************************************/

#pragma once

#include <actl/io/util/serialization_access.hpp>
#include <actl/std/tuple.hpp>
#include <actl/util/none.hpp>
#include <actl/util/span.hpp>
#include <actl/util/type_traits.hpp>
#include <cstdint>

namespace ac::io {

/* Device */

using mode_t = uint8_t;
inline constexpr mode_t bin = 0x01;
inline constexpr mode_t in = 0x02;
inline constexpr mode_t out = 0x04;
inline constexpr mode_t app = 0x08;
inline constexpr mode_t line_buffered = 0x10;
inline constexpr mode_t trunc = in | out | app;

template <mode_t Mode>
inline constexpr bool is_bin = (Mode & bin) > 0;

template <mode_t Mode>
inline constexpr bool is_in = (Mode & in) > 0;

template <mode_t Mode>
inline constexpr bool is_out = (Mode & (out | app)) > 0;

template <mode_t Mode>
inline constexpr bool is_line_buffered = (Mode & line_buffered) > 0;

template <class Device>
using char_t = typename Device::char_type;

struct device_base {};

template <mode_t Mode, class Char>
struct device : device_base {
    static_assert(is_in<Mode> || is_out<Mode>, "invalid mode");

    using char_type = Char;

    static constexpr mode_t mode = Mode;
};

template <class T>
inline constexpr bool is_device_v = std::is_base_of_v<device_base, T>;

template <class T, class = void>
struct has_input_buffer : std::false_type {};

template <class T>
struct has_input_buffer<T, std::void_t<decltype(std::declval<T>().input_data())>> : std::true_type {
};

template <class T, class = void>
struct has_output_buffer : std::false_type {};

template <class T>
struct has_output_buffer<T, std::void_t<decltype(std::declval<T>().output_data())>>
    : std::true_type {};

/* Format */

struct binary {
    using format_tag = binary;
    using base = none;
};

struct text_tag {
    using base = none;
};

template <class T, class Tag, class = void>
struct has_format_tag : std::false_type {};

template <class T, class Tag>
struct has_format_tag<T, Tag, std::void_t<typename T::format_tag>>
    : std::is_same<typename T::format_tag, Tag> {};

template <class T>
using enable_int_if_text = enable_int_if<has_format_tag<T, text_tag>::value>;

template <class T, class = void>
struct format_traits {
    static constexpr size_t size = 0;
};

template <class T>
inline constexpr bool is_format_v = format_traits<T>::size > 0;

template <class T>
struct format_traits<T, std::void_t<typename T::format_tag>> {
    static constexpr size_t size = 1;

    template <size_t I>
    static T& get(T& x) {
        static_assert(I == 0);
        return x;
    }
};

template <class... Ts>
struct format_traits<std::tuple<Ts...>, void> {
    static constexpr size_t size =
        (... && is_format_v<std::remove_reference_t<Ts>>) ? sizeof...(Ts) : 0;

    template <size_t I, class T>
    static auto& get(T& x) {
        return std::get<I>(x);
    }
};

template <class Format>
inline void change_depth(Format&, bool deeper) {}

template <class Format, size_t... Is>
inline void change_depth(Format& fmt, bool deeper, std::index_sequence<Is...>) {
    return (..., change_depth(std::get<Is>(fmt), deeper));
}

template <class... Formats>
inline void change_depth(std::tuple<Formats...>& fmt, bool deeper) {
    change_depth(fmt, deeper, std::make_index_sequence<sizeof...(Formats)>{});
}

template <class Device, enable_int_if<is_bin<Device::mode>> = 0>
inline binary deduce_format(Device& dev) {
    return {};
}

/* Argument traits */

template <class... Ts>
struct tuple : std::tuple<Ts...> {
    using std::tuple<Ts...>::tuple;
};

template <class... Ts>
tuple(Ts&&...) -> tuple<Ts...>;

template <class T>
struct is_tuple : decltype(serialization_access{}.is_io_tuple<T>(0)) {};

template <class... Ts>
struct is_tuple<std::tuple<Ts...>> : std::true_type {};

template <class T>
inline constexpr bool is_tuple_v = is_tuple<T>::value;

template <class T, class = void>
struct is_manipulator : std::false_type {};

template <class T>
struct is_manipulator<T, std::void_t<typename T::is_manipulator>> : std::true_type {};

/* Common types support */

template <class Device, class Format, class T, enable_int_if<std::is_empty_v<T>> = 0>
inline index write_final(Device&, Format&, const T&) {
    return 0;
}

template <class Device, class Format, class T, enable_int_if<std::is_empty_v<T>> = 0>
inline bool read_final(Device&, Format&, T&) {
    return true;
}

template <class T>
using enable_int_if_byte = enable_int_if<std::is_arithmetic_v<T> && sizeof(T) == 1>;

template <class Device, class Format, class B, enable_int_if_byte<B> = 0>
inline index write_final(Device& od, Format&, B byte) {
    return od.write(static_cast<char_t<Device>>(byte));
}

template <class Device, class Format, class B, enable_int_if_byte<B> = 0>
inline bool read_final(Device& id, Format&, B& byte) {
    byte = static_cast<B>(id.get());
    return !id.eof();
}

template <class Device, class Format, class B, index N, enable_int_if_byte<B> = 0>
inline index write_final(Device& od, Format&, span<B, N> s) {
    return od.write({reinterpret_cast<const char_t<Device>*>(s.data()), s.size()});
}

template <class Device, class Format, class B, index N, enable_int_if_byte<B> = 0>
inline bool read_final(Device& id, Format&, span<B, N>& s) {
    return id.read({reinterpret_cast<char_t<Device>*>(s.data()), s.size()}) == s.size();
}

template <class Device, class Format, class B, index N, enable_int_if_byte<B> = 0>
inline bool read_final(Device& id, Format&, cspan<B, N>& s) {
    for (auto c : s) {
        if (static_cast<B>(id.peek()) != c) return false;
        id.move(1);
    }
    return true;
}

/* Processing argument with multiple formats */

namespace detail {

template <class... T>
constexpr auto can_serialize(T&... xs) -> decltype(serialize(xs...), std::true_type{});

constexpr std::false_type can_serialize(...);

template <size_t I, class D, class F, class T>
inline index write_impl(D& od, F& fmt, const T& x);

template <size_t I, class D, class F, class T, size_t... Is>
inline index write_impl_tuple(D& od, F& fmt, const T& x, std::index_sequence<Is...>) {
    return (... + write_impl<I>(od, fmt, std::get<Is>(x)));
}

template <size_t I, class D, class F, class... Ts>
inline index write_impl(D& od, F& fmt, const tuple<Ts...>& x) {
    return write_impl_tuple<I>(od, fmt, x, std::make_index_sequence<sizeof...(Ts)>{});
}

template <size_t I, class D, class F, class T>
inline index write_impl(D& od, F& fmt, const T& x) {
    if constexpr (I == format_traits<F>::size) {
        if constexpr (is_range_v<T> || is_tuple_v<T>) {
            change_depth(fmt, true);
            index res = write_final(od, fmt, x);
            change_depth(fmt, false);
            return res;
        } else {
            return write_final(od, fmt, x);
        }
    } else {
        auto& fmt_i = format_traits<F>::template get<I>(fmt);
        if constexpr (!decltype(can_serialize(fmt_i, x))::value) {
            return write_impl<I + 1>(od, fmt, x);
        } else if constexpr (std::is_same_v<decltype(serialize(fmt_i, x)), void>) {
            serialize(fmt_i, x);
            return 0;
        } else {
            return write_impl<I + 1>(od, fmt, serialize(fmt_i, x));
        }
    }
}

template <class... Ts>
constexpr auto can_deserialize(Ts&... xs) -> decltype(deserialize(xs...), std::true_type{});

constexpr std::false_type can_deserialize(...);

template <size_t I, class D, class F, class T>
inline bool read_impl(D& od, F& fmt, T& x);

template <size_t I, class D, class F, class T, size_t... Is>
inline bool read_impl_tuple(D& od, F& fmt, T& x, std::index_sequence<Is...>) {
    return (... && read_impl<I>(od, fmt, std::get<Is>(x)));
}

template <size_t I, class D, class F, class... Ts>
inline bool read_impl(D& od, F& fmt, tuple<Ts...>& x) {
    return read_impl_tuple<I>(od, fmt, x, std::make_index_sequence<sizeof...(Ts)>{});
}

template <size_t I, class D, class F, class T>
inline bool read_impl(D& od, F& fmt, T& x) {
    if constexpr (I == format_traits<F>::size) {
        if constexpr (is_range_v<T> || is_tuple_v<T>) {
            change_depth(fmt, true);
            bool res = read_final(od, fmt, x);
            change_depth(fmt, false);
            return res;
        } else {
            return read_final(od, fmt, x);
        }
    } else {
        auto& fmt_i = format_traits<F>::template get<I>(fmt);
        if constexpr (decltype(can_deserialize(od, fmt_i, x))::value) {
            return deserialize(od, fmt_i, x);
        } else if constexpr (decltype(can_deserialize(fmt_i, x))::value) {
            if constexpr (std::is_same_v<decltype(deserialize(fmt_i, x)), void>) {
                deserialize(fmt_i, x);
                return true;
            } else {
                decltype(auto) y = deserialize(fmt_i, x);
                return read_impl<I + 1>(od, fmt, y);
            }
        } else {
            return read_impl<I + 1>(od, fmt, x);
        }
    }
}

}  // namespace detail

/* Read and write. Absence of std::forward is intentional here to convert rvalue references into
 * lvalue references, because I/O doesn't operate with rvalues. */

template <class Device, class Format, class... Ts>
inline index write(Device&& od, Format&& fmt, const Ts&... args) {
    if constexpr (is_format_v<std::remove_reference_t<Format>>) {
        return (... + detail::write_impl<0>(od, fmt, args));
    } else {
        return write(od, deduce_format(od), fmt, args...);
    }
}

template <class Device, class Format, class... Ts>
inline bool read(Device&& id, Format&& fmt, Ts&&... args) {
    if constexpr (is_format_v<std::remove_reference_t<Format>>) {
        return (... && detail::read_impl<0>(id, fmt, args));
    } else {
        return read(id, deduce_format(id), fmt, args...);
    }
}

template <class Device, class Format, class T>
inline index write_size(Device& od, Format& fmt, const T& size) {
    return write(od, fmt, size);
}

template <class Device, class Format, class T>
inline bool read_size(Device& id, Format& fmt, T& size) {
    return read(id, fmt, size);
}

}  // namespace ac::io
