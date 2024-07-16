#pragma once

#include "namespace_.hxx"
#include "text/namespace_.hxx"

#include "text/type+fwd.hxx"
#include "text/chunk+fwd.hxx"
#include "text/driver+fwd.hxx"


namespace p5::lambda::utils::log::message {
namespace text {
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

namespace normailzer {

namespace parent_ = this_;
namespace this_ = parent_::normailzer;

namespace driver {

namespace parent_ = this_;
namespace this_ = parent_::driver;

using Chunk = parent_::parent_::Chunk;
using Interface = parent_::parent_::driver::Interface;

template <class> struct Type;

template <class T> constexpr auto make(T &&) noexcept(true);

} // namespace driver

template <class T> using Driver = driver::Type<T>;

template <class T> constexpr auto make(T &&) noexcept(true);

} // namespace normailzer

template <class T> constexpr auto make(T &&) noexcept(true);

} // namespace text

using Text = this_::text::Type;

} // namespace p5::lambda::utils::log::message
