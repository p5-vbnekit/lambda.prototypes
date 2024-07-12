#pragma once

#include "message.hxx"

#include "raii+fwd.hxx"


namespace p5::lambda::utils::log::raii {

struct Type final {
    using Target = this_::Target;
    using Message = this_::Message;

    Target *target;
    Message message = {};

    explicit Type(Target * = nullptr) noexcept(true);
    explicit Type(Message &&, Target * = nullptr) noexcept(true);
    explicit Type(Message const &, Target * = nullptr) noexcept(true);

    Type(Target *, Message &&) noexcept(true);
    Type(Target *, Message const &) noexcept(true);

    ~Type() noexcept(true);

    Type(Type &&) noexcept(true);
    Type & operator = (Type &&) noexcept(true);

    template <class T> auto & operator << (T &&) noexcept(false);

private:
    Type(Type const &) = delete;
    Type & operator = (Type const &) = delete;
};

} // namespace p5::lambda::utils::log::raii


#include "raii+inl.hxx"
