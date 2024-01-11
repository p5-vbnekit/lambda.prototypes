#pragma once

#include "namespace_.hxx"


namespace p5::lambda::utils::log {
namespace message {

namespace parent_ = this_;
namespace this_ = parent_::message;

struct Type;

namespace text {

namespace parent_ = this_;
namespace this_ = parent_::text;

} // namespace text

namespace level {

namespace parent_ = this_;
namespace this_ = parent_::level;

enum class Type;

template <Type> constexpr auto const * to_string() noexcept(true);
auto const * to_string(Type const &) noexcept(false);

} // namespace level

using Level = level::Type;

namespace location {

namespace parent_ = this_;
namespace this_ = parent_::location;

} // namespace location

namespace formatter {

namespace parent_ = this_;
namespace this_ = parent_::formatter;

struct Type;

using Input = parent_::Type;

} // namespace formatter

using Formatter = formatter::Type;

} // namespace message

using Message = message::Type;

} // namespace p5::lambda::utils::log
