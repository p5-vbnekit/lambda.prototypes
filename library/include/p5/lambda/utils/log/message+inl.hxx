#pragma once

#include <stdexcept>
#include <unordered_map>

#include "message.hxx"


namespace p5::lambda::utils::log::message {
namespace level {

template <Type> struct ToString_ final {};

template <> struct ToString_<Type::Error> final {
    inline constexpr static auto const * const value = "Error";
};

template <> struct ToString_<Type::Warning> final {
    inline constexpr static auto const * const value = "Warning";
};

template <> struct ToString_<Type::Info> final {
    inline constexpr static auto const * const value = "Info";
};

template <> struct ToString_<Type::Debug> final {
    inline constexpr static auto const * const value = "Debug";
};

template <> struct ToString_<Type::Developer> final {
    inline constexpr static auto const * const value = "Developer";
};

template <Type value> inline constexpr auto const * to_string() noexcept(true) {
    return ToString_<value>::value;
}

inline auto const * to_string(Type const &value) noexcept(false) {
    thread_local static auto const map_ = ::std::unordered_map<
        Type, ::std::string_view
    > {
        {Type::Error, to_string<Type::Error>()},
        {Type::Warning, to_string<Type::Warning>()},
        {Type::Info, to_string<Type::Info>()},
        {Type::Developer, to_string<Type::Developer>()},
        {Type::Debug, to_string<Type::Debug>()}
    };

    auto const iterator_ = map_.find(value);
    if (iterator_ == map_.end()) throw ::std::invalid_argument{"key not found"};
    return iterator_->second.data();
}

} // namespace level

namespace location {

inline constexpr auto unknown(Type const &value) noexcept(true) {
    constexpr auto const empty_ = Type{};
    auto const * const file_ = value.file_name();
    if (static_cast<bool>(file_) && static_cast<bool>(*file_) && (
            empty_.file_name() != file_)
    ) return false;
    auto const * const function_ = value.function_name();
    return ! (static_cast<bool>(function_) && static_cast<bool>(*function_) && (
        empty_.function_name() != function_
    ));
}

} // namespace location
} // namespace p5::lambda::utils::log::message
