#pragma once

#include "namespace_.hxx"
#include "log/namespace_.hxx"

#include "log/raii+fwd.hxx"
#include "log/handler+fwd.hxx"
#include "log/message+fwd.hxx"


namespace p5::lambda::utils {
namespace log {

struct Type;

} // namespace log

using Log = log::Type;

} // namespace p5::lambda::utils
