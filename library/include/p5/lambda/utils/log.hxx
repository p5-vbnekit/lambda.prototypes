#pragma once

#include <type_traits>
#include <unordered_map>
#include <source_location>

#ifndef __cpp_lib_source_location
#include <experimental/source_location>
#endif

#include "log+fwd.hxx"

#include "log/raii.hxx"
#include "log/handler.hxx"
#include "log/message.hxx"


namespace p5::lambda::utils::log {

struct Type final {
    using RAII = this_::RAII;
    using Handler = this_::Handler;
    using Message = this_::Message;

    Handler handler;

    template <Message::Level, class T> auto write(
        T &&, Message::Location && = Message::Location::current()
    ) noexcept(true);

    template <class T> auto write(
        Message::Level const &, T &&,
        Message::Location && = Message::Location::current()
    ) noexcept(true);

    void write(
        Message::Text const &,
        Message::Level const &level = Message::Level::Info,
        Message::Location && = Message::Location::current()
    ) noexcept(true);

    void write(Message const &) noexcept(true);

    template <class T> requires(::std::is_convertible_v<T, Handler>)
    auto operator >> (T &&) noexcept(false);

    template <class T> auto operator << (T &&) noexcept(false);
};

} // namespace p5::lambda::utils::log


#include "log+inl.hxx"
