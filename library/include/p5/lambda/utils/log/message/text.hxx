#pragma once

#include <list>
#include <tuple>
#include <ranges>
#include <variant>
#include <utility>
#include <iterator>
#include <exception>
#include <string_view>
#include <type_traits>

#include "../../generator.hxx"

#include "text+fwd.hxx"


namespace p5::lambda::utils::log::message::text {
namespace chunk {

struct Type final {
    ::std::string_view reference = {};
    bool terminated = false;
};

} // namespace chunk

namespace view {
namespace policy {
namespace line {

enum class Type { Copy, Chunk, Chunks, Default = Chunks };

} // namespace line

struct Default final {
    inline constexpr static auto const line = this_::Line::Default;
    bool terminate = false;
};

template <class T> class Traits final {
    static_assert(::std::is_same_v<T, ::std::decay_t<T>>);

    inline constexpr static auto const default_ = this_::Default{};

public:
    constexpr static this_::Line line() noexcept(true);
    constexpr static bool terminate(T const &value) noexcept(true);
};

} // namespace policy

namespace function {

template <class> struct Type final {
    constexpr auto operator () () const noexcept(true);
};

} // namespace function
} // namespace view

struct Type final {
    using Chunk = this_::Chunk;

    template <class = void> auto view() const noexcept(true);
    template <class Policy> auto view(Policy &&) const noexcept(true);

    template <class ... T> auto emplace(T && ...) noexcept(false);

    auto operator * () const noexcept(true);

    Type() = default;

    template <class ... T> explicit Type(T && ... source) requires([] {
        if constexpr (1 < sizeof ... (T)) return true;
        else if constexpr (0 < sizeof ... (T)) return [] {
            return ! ::std::is_base_of_v<Type, ::std::decay_t<decltype(
                ::std::get<0>(::std::make_tuple(::std::declval<T>() ...))
            )>>;
        } ();
        else return false;
    } ()) { emplace(::std::forward<T>(source) ...); }

    Type(Type &&) noexcept(true) = default;
    Type(Type const &) = default;

    Type & operator = (Type &&) noexcept(true) = default;
    Type & operator = (Type const &) = default;

private:
    ::std::list<Chunk> chunks_ = {};
};

} // namespace p5::lambda::utils::log::message::text


#include "text+inl.hxx"
