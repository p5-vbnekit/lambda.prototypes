#pragma once

#include <utility>
#include <type_traits>

#include "message.hxx"
#include "type.hxx"


namespace p5::lambda::utils::log {

template <this_::Type::Message::Level level> inline auto this_::Type::write(
    FinalToken_ &&, Message::Location &&location
) noexcept(true) {
    auto &&message_ = Message{.level = level};
    if constexpr (
        Message::Level::Info < level
    ) message_.location = ::std::move(location);
    return RAII{this, ::std::move(message_)};
}

template <this_::Type::Message::Level level, class Location> inline
auto this_::Type::write(Location &&location) noexcept(true) requires(
    Speculations_::location<Location>()
) {
    return RAII{this, {
        .level = level, .location = ::std::forward<Location>(location)
    }};
}

template <class Location> inline auto this_::Type::write(
    this_::Type::Message::Level level, Location &&location
) noexcept(true) requires(Speculations_::location<Location>()) {
    return RAII{this, {
        .level = level, .location = ::std::forward<Location>(location)
    }};
}

template <this_::Type::Message::Level level, class Text> inline
auto this_::Type::write(
    Text &&text, FinalToken_ &&, Message::Location &&location
) requires(Speculations_::text<Text>()) {
    auto &&message_ = Message{.text = this_::message::text::make(
        ::std::forward<Text>(text)
    ), .level = level};
    if constexpr (
        Message::Level::Info < level
    ) message_.location = ::std::move(location);
    write(::std::move(message_));
}

template <this_::Type::Message::Level level, class Text, class Location> inline
auto this_::Type::write(Text &&text, Location &&location) requires(
    Speculations_::text<Text>() && Speculations_::location<Location>()
) {
    write(Message{
        .text = this_::message::text::make(::std::forward<Text>(text)),
        .level = level, .location = ::std::forward<Location>(location)
    });
}

template <class Text> inline auto this_::Type::write(
    Message::Level level, Text &&text,
    FinalToken_ &&, Message::Location &&location
) requires(Speculations_::text<Text>()) {
    auto &&message_ = Message{.text = this_::message::text::make(
        ::std::forward<Text>(text)
    ), .level = level};
    if (Message::Level::Info < level) message_.location = ::std::move(location);
    write(::std::move(message_));
}

template <class Text, class Location> inline auto this_::Type::write(
    Message::Level level, Text &&text, Location &&location
) requires(
    Speculations_::text<Text>() && Speculations_::location<Location>()
) {
    write(Message{
        .text = this_::message::text::make(::std::forward<Text>(text)),
        .level = level, .location = ::std::forward<Location>(location)
    });
}

template <class Text> inline auto this_::Type::write(
    Text &&text, Message::Level level,
    FinalToken_ &&, Message::Location &&location
) requires(Speculations_::text<Text>()) {
    auto &&message_ = Message{.text = this_::message::text::make(
        ::std::forward<Text>(text)
    ), .level = level};
    if (Message::Level::Info < level) message_.location = ::std::move(location);
    write(::std::move(message_));
}

template <class Text, class Location> inline auto this_::Type::write(
    Text &&text, Message::Level level, Location &&location
) requires(
    Speculations_::text<Text>() && Speculations_::location<Location>()
) {
    write(Message{
        .text = this_::message::text::make(::std::forward<Text>(text)),
        .level = level, .location = ::std::forward<Location>(location)
    });
}

template <class T> inline
auto Type::operator >> (T &&handler) noexcept(false) requires(
    ::std::is_convertible_v<T, Handler>
) {
    this->handler = ::std::forward<T>(handler);
}

} // namespace p5::lambda::utils::log
