#pragma once

#include <utility>

#include "log.hxx"


namespace p5::lambda::utils::log {

template <Message::Level level, class T>
inline auto Type::write(T &&text, Message::Location &&location) noexcept(true) {
    return write(::std::forward<T>(text), level, ::std::move(location));
}

template <class T> inline auto Type::write(
    Message::Level const &level, T &&text,
    Message::Location &&location
) noexcept(true) {
    return write(::std::forward<T>(text), level, ::std::move(location));
}

template <class T> requires(::std::is_convertible_v<T, Handler>)
inline auto Type::operator >> (T &&handler) noexcept(false) {
    this->handler = ::std::forward<T>(handler);
}

template <class T> inline auto Type::operator << (T &&some) noexcept(false) {
    return RAII{} << ::std::forward<T>(some);
}

} // namespace p5::lambda::utils::log
