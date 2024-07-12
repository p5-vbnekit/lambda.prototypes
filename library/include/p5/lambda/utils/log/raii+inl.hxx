#pragma once

#include <utility>

#include "raii.hxx"


namespace p5::lambda::utils::log::raii {

template <class T> auto & Type::operator << (T &&text) noexcept(false) {
    message.text.emplace(::std::forward<T>(text));
    return *this;
}

} // namespace p5::lambda::utils::log::raii
