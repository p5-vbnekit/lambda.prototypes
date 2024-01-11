#pragma once

#include "namespace_.hxx"

#include "game+fwd.hxx"
#include "engine+fwd.hxx"
#include "metamod+fwd.hxx"


namespace p5::lambda::api {

namespace parent_ = this_;
namespace this_ = parent_::api;

template <class> constexpr static auto version() noexcept(true);
constexpr static void version(void const *) noexcept(true) = delete;

} // namespace p5::lambda::api
