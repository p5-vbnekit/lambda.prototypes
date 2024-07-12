#pragma once

#include "location.hxx"


namespace p5::lambda::utils::log::message::location {

inline constexpr static auto unknown(Type const &value) noexcept(true) {
    constexpr auto const empty_ = Type{};
    auto const * const file_ = value.file_name();
    if (file_ && *file_ && (empty_.file_name() != file_)) return false;
    auto const * const function_ = value.function_name();
    return ! (function_ && *function_ && (empty_.function_name() != function_));
}

} // namespace p5::lambda::utils::log::message::location
