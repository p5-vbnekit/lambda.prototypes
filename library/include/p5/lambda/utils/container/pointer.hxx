#pragma once

#include <memory>
#include <type_traits>

#include "pointer+fwd.hxx"


namespace p5::lambda::utils::container::pointer {

template <class Interface> struct Type final {
    static_assert(! ::std::is_pointer_v<Interface>);
    static_assert(! ::std::is_reference_v<Interface>);

    using Driver = this_::Driver;

    operator bool () const noexcept(true);
    auto & operator * () const noexcept(false);

    template <class Handler>
    auto operator () (Handler &&) const noexcept(false);

    explicit Type(::std::shared_ptr<Driver> &&) noexcept(true);

    Type() noexcept(true) = default;
    Type(Type &&) noexcept(true) = default;
    Type(Type const &) noexcept(true) = default;
    Type & operator = (Type &&) noexcept(true) = default;
    Type & operator = (Type const &) noexcept(true) = default;

private:
    mutable ::std::weak_ptr<Driver> driver_ = {};
};

} // namespace p5::lambda::utils::container::pointer


#include "pointer+inl.hxx"
