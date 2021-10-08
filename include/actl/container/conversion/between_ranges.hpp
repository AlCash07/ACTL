#pragma once

#include <actl/container/conversion/convert_to.hpp>
#include <actl/range/is_dynamic_range.hpp>

namespace ac {

template <class To, class From>
struct conversion_sfinae<
    std::enable_if_t<is_dynamic_range_v<To> && is_dynamic_range_v<From>>,
    To,
    From>
{
    using from_iter = iterator_t<std::remove_reference_t<const From&>>;

    static constexpr bool value =
        std::is_constructible_v<To, from_iter, from_iter>;

    static constexpr To convert(const From& x) noexcept(noexcept(To{
        std::begin(x), std::end(x)}))
    {
        return To{std::begin(x), std::end(x)};
    }
};

} // namespace ac
