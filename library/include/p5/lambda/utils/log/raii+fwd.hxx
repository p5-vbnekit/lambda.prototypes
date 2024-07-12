#pragma once

#include "namespace_.hxx"

#include "type+fwd.hxx"
#include "message+fwd.hxx"


namespace p5::lambda::utils::log {
namespace raii {

namespace parent_ = this_;
namespace this_ = parent_::raii;

struct Type;

using Target = parent_::Type;
using Message = parent_::Message;

} // namespace raii

using RAII = raii::Type;

} // namespace p5::lambda::utils::log
