#pragma once

#include <type_traits>

#include "type+fwd.hxx"

#include "raii.hxx"
#include "handler.hxx"
#include "message.hxx"


namespace p5::lambda::utils::log {

struct Type final {
    using RAII = this_::RAII;
    using Handler = this_::Handler;
    using Message = this_::Message;

    Handler handler;

private:
    struct FinalToken_ {};

    struct Speculations_ final {
        template <class T> inline constexpr
        static auto location() noexcept(true) {
            return ::std::is_base_of_v<Message::Location, ::std::decay_t<T>>;
        }

        template <class T> inline constexpr
        static auto text() noexcept(true) {
            if (location<T>()) return false;
            using Suspect_ = ::std::decay_t<T>;
            if (::std::is_base_of_v<Message, Suspect_>) return false;
            if (::std::is_base_of_v<Message::Level, Suspect_>) return false;
            return true;
        }
    };

public:
    template <Message::Level = Message::Level::Info> auto write(
        FinalToken_ && = {}, Message::Location && = Message::Location::current()
    ) noexcept(true);

    template <
        Message::Level = Message::Level::Info, class Location
    > auto write(Location &&) noexcept(true) requires(
        Speculations_::location<Location>()
    );

    RAII write(
        Message::Level, FinalToken_ && = {},
        Message::Location && = Message::Location::current()
    ) noexcept(true);

    template <class Location> auto write(
        Message::Level, Location &&
    ) noexcept(true) requires(Speculations_::location<Location>());

    template <Message::Level = Message::Level::Info, class Text> auto write(
        Text &&, FinalToken_ && = {},
        Message::Location && = Message::Location::current()
    ) requires(Speculations_::text<Text>());

    template <
        Message::Level = Message::Level::Info, class Text, class Location
    > auto write(Text &&, Location &&) requires(
        Speculations_::text<Text>() && Speculations_::location<Location>()
    );

    template <class Text> auto write(
        Message::Level, Text &&, FinalToken_ && = {},
        Message::Location && = Message::Location::current()
    ) requires(Speculations_::text<Text>());

    template <class Text, class Location> auto write(
        Message::Level, Text &&, Location &&
    ) requires(
        Speculations_::text<Text>() && Speculations_::location<Location>()
    );

    template <class Text> auto write(
        Text &&, Message::Level, FinalToken_ && = {},
        Message::Location && = Message::Location::current()
    ) requires(Speculations_::text<Text>());

    template <class Text, class Location> auto write(
        Text &&, Message::Level, Location &&
    ) requires(
        Speculations_::text<Text>() && Speculations_::location<Location>()
    );

    void write(Message const &) noexcept(true);

    template <class T> auto operator >> (T &&) noexcept(false) requires(
        ::std::is_convertible_v<T, Handler>
    );
};

} // namespace p5::lambda::utils::log


#include "type+inl.hxx"
