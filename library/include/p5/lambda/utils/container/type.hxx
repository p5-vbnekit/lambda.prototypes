#pragma once

#include <memory>
#include <exception>
#include <type_traits>

#include "lock+fwd.hxx"
#include "type+fwd.hxx"
#include "driver+fwd.hxx"
#include "pointer+fwd.hxx"


namespace p5::lambda::utils::container {

struct Type final {
    template <class T> using Lock = this_::Lock<T>;
    template <class T> using Pointer = this_::Pointer<T>;

    template <class Interface>
    auto get() const noexcept(true);

    template <class Interface, class Handler>
    auto get(Handler &&) const noexcept(false);

    template <class Interface> auto lock() noexcept(false);

    template <class Interface, class Factory>
    auto inject(Factory &&) noexcept(false);

    template <class Instance>
    auto assign(Instance &) noexcept(false);

    template <class Interface, class Instance>
    auto assign(Instance &) noexcept(false);

    template <class Instance, class ... T>
    auto emplace(T && ...) noexcept(false);

    template <class Interface, class Instance, class ... T>
    auto emplace(T && ...) noexcept(false);

    auto collect() noexcept(true);

    template <class Handler> auto on_error(Handler &&) noexcept(false);

    template <class Interface, class Handler>
    auto on_error(Handler &&) noexcept(false);

    Type() noexcept(true);
    Type(Type &&) noexcept(true) = default;
    Type & operator = (Type &&) noexcept(true) = default;

    ~Type() noexcept(true);

private:
    ::std::unique_ptr<this_::Driver> driver_;

    Type(Type const &) = delete;
    Type & operator = (Type const &) = delete;
};

} // namespace p5::lambda::utils::container


#include "type+inl.hxx"
