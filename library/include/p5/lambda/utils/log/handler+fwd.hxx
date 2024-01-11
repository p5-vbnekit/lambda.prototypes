#pragma once

#include "namespace_.hxx"

#include "message+fwd.hxx"


namespace p5::lambda::utils::log::handler {

namespace parent_ = this_;
namespace this_ = parent_::handler;

using Message = parent_::Message;

} // namespace p5::lambda::utils::log::handler
