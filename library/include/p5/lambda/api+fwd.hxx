#pragma once

#include "namespace_.hxx"


namespace p5::lambda::api {

namespace parent_ = this_;
namespace this_ = parent_::api;

template <class> constexpr auto version() noexcept(true);
constexpr void version(void const *) noexcept(true) = delete;

} // namespace p5::lambda::api
