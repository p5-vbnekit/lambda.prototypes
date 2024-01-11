#pragma once

#include <string>
#include <optional>
#include <string_view>

#ifdef __cpp_lib_source_location
#include <source_location>
#else
#include <experimental/source_location>
#endif

#include "message+fwd.hxx"


namespace p5::lambda::utils::log::message {
namespace text {

using Type = ::std::string_view;

} // namespace text

using Text = text::Type;

namespace level {

enum class Type { Error, Warning, Info, Debug, Developer };

} // namespace level

namespace location {

#ifdef __cpp_lib_source_location
using Type = ::std::source_location;
#else
using Type = ::std::experimental::source_location;
#endif

constexpr static auto unknown(Type const &) noexcept(true);

} // namespace location

using Location = location::Type;

struct Type final {
    using Text = this_::Text;
    using Level = this_::Level;
    using Location = this_::Location;

    Text text = {};
    Level level = Level::Info;
    ::std::optional<Location> location = ::std::nullopt;
};

namespace formatter {

using Tag = parent_::Text;
using Path = parent_::Text;
using Output = ::std::string;

struct Type final {
    using Tag = this_::Tag;
    using Path = this_::Path;
    using Input = this_::Input;
    using Output = this_::Output;

    Tag tag = {};
    Path base = {};
    bool level = true;
    bool location = true;

    Output operator () (Input const &) const noexcept(false);
};

} // namespace formatter
} // namespace p5::lambda::utils::log::message


#include "message+inl.hxx"
