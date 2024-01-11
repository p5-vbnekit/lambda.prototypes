#pragma once

#include "raii.hxx"


namespace p5::lambda::utils::log::raii {

template <class T> auto & Type::operator << (T &&/*something*/) noexcept(false) {
    return *this;
}

} // namespace p5::lambda::utils::log::raii
