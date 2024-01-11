#pragma once

#include "namespace_.hxx"


namespace p5::lambda::utils::log {
namespace raii {

namespace parent_ = this_;
namespace this_ = parent_::raii;

struct Type;

} // namespace raii

using RAII = raii::Type;

} // namespace p5::lambda::utils::log
