#pragma once

#include <memory>
#include <type_traits>

#include "lock+fwd.hxx"


namespace p5::lambda::utils::container::lock {

template <class Interface> struct Type final {
    static_assert(! ::std::is_pointer_v<Interface>);
    static_assert(! ::std::is_reference_v<Interface>);

    using Driver = this_::Driver;

    auto && take() noexcept(true);
    auto release() noexcept(true);

    template <class Factory> auto inject(Factory &&) noexcept(false);
    template <class Instance> auto assign(Instance &) noexcept(false);

    template <class Instance, class ... T>
    auto emplace(T && ...) noexcept(false);

    explicit Type(::std::unique_ptr<Driver> &&) noexcept(true);

    Type(Type &&) noexcept(true) = default;
    Type & operator = (Type &&) noexcept(true) = default;

private:
    ::std::unique_ptr<Driver> driver_ = {};

    Type() = delete;
    Type(Type const &) = delete;
    Type & operator = (Type const &) = delete;
};

} // namespace p5::lambda::utils::container::lock


#include "lock+inl.hxx"
