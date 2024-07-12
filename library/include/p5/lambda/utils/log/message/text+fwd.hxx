#pragma once

#include "namespace_.hxx"


namespace p5::lambda::utils::log::message {
namespace text {

namespace parent_ = this_;
namespace this_ = parent_::text;

namespace chunk {

namespace parent_ = this_;
namespace this_ = parent_::chunk;

struct Type;

template <class ... T> constexpr static auto make(T && ...) noexcept(true);

} // namespace chunk

using Chunk = chunk::Type;

namespace view {

namespace parent_ = this_;
namespace this_ = parent_::view;

namespace policy {

namespace parent_ = this_;
namespace this_ = parent_::policy;

namespace line {

namespace parent_ = this_;
namespace this_ = parent_::line;

enum class Type;

namespace types {

namespace parent_ = this_;
namespace this_ = parent_::types;

} // namespace types
} // namespace line

using Line = line::Type;

struct Default;

template <class = Default> class Traits;

} // namespace policy

namespace function {

namespace parent_ = this_;
namespace this_ = parent_::function;

template <class Policy = void> struct Type;

} // namespace function

template <class Policy> using Function = function::Type<Policy>;

} // namespace view

struct Type;

template <class ... T> static auto make(T && ...) noexcept(true);

} // namespace text

using Text = text::Type;

} // namespace p5::lambda::utils::log::message
