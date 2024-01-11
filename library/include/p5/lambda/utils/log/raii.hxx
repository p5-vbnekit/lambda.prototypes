#pragma once

#include "raii+fwd.hxx"


namespace p5::lambda::utils::log::raii {

struct Type final {
    template <class T> auto & operator << (T &&) noexcept(false);
};

} // namespace p5::lambda::utils::log::raii


#include "raii+inl.hxx"
