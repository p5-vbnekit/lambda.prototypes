#pragma once

#include <type_traits>

#include "game.hxx"


namespace p5::lambda::game {

inline constexpr static auto api::version(functions::Standard const *) noexcept(true) { return 140; }
inline constexpr static auto api::version(functions::Extension const *) noexcept(true) { return 1; }

template <class T> inline constexpr static auto api::version() noexcept(true) {
    return version(static_cast<::std::decay_t<T> const *>(nullptr));
}

} // namespace p5::lambda::game
