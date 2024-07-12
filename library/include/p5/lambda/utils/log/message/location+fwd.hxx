#pragma once

#include <string>
#include <string_view>

#ifdef __cpp_lib_source_location
#include <source_location>
#else
#include <experimental/source_location>
#endif

#include "namespace_.hxx"


namespace p5::lambda::utils::log::message {
namespace location {

namespace parent_ = this_;
namespace this_ = parent_::location;

#ifdef __cpp_lib_source_location
using Type = ::std::source_location;
#else
using Type = ::std::experimental::source_location;
#endif

constexpr static auto unknown(Type const &) noexcept(true);

::std::string to_string(
    Type const &, ::std::string_view const &root = {}
) noexcept(false);

} // namespace location

using Location = location::Type;

} // namespace p5::lambda::utils::log::message
