#pragma once

#include <functional>

#include "handler+fwd.hxx"


namespace p5::lambda::utils::log {
namespace handler {

using Type = ::std::function<void(Message const &)>;

} // namespace handler

using Handler = handler::Type;

} // namespace p5::lambda::utils::log
