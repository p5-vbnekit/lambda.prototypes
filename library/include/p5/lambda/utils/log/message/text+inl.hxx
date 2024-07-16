#pragma once

#include <utility>
#include <type_traits>

#include "../../generator.hxx"

#include "text.hxx"


namespace p5::lambda::utils::log::message::text {
namespace view {
namespace policy {

template <class T> inline constexpr Line Traits<T>::line() noexcept(true) {
    if constexpr (requires () { T::line::value; }) return T::line::value;
    else if constexpr (requires() { T::line; }) return T::line;
    else return default_.line;
}

template <class T> inline constexpr bool
Traits<T>::terminate(T const &value) noexcept(true) {
    if constexpr (
        requires () { T::terminate::value; }
    ) return T::terminate::value;
    else if constexpr (
        requires() { value.terminate; }
    ) return value.terminate;
    else return default_.terminate;
}

} // namespace policy

namespace function {

template <class T> inline constexpr
auto this_::Type<T>::operator () () const noexcept(true) {
    return static_cast<parent_::parent_::Chunk const *>(nullptr);
}

} // namespace function
} // namespace view

namespace normailzer {
namespace driver {

template <class T> inline constexpr auto make(T &&source) noexcept(true) {
    using Source_ = ::std::remove_reference_t<T>;
    if constexpr (::std::is_pointer_v<Source_>) {
        using Type_ = ::std::remove_reference_t<
            decltype(*::std::declval<Source_>())
        >;
        if constexpr (::std::is_base_of_v<
            this_::Interface, Type_
        >) return this_::Type<this_::Interface *>{::std::forward<T>(source)};
        else return this_::Type<Source_>{::std::forward<T>(source)};
    }
    else if constexpr (::std::is_reference_v<T>) {
        if constexpr (::std::is_base_of_v<
            this_::Interface, Source_
        >) return this_::Type<this_::Interface *>{&source};
        else return this_::Type<Source_>{&source};
    }
    else return this_::Type<Source_>{::std::forward<T>(source)};
}

} // namespace driver

template <class T> inline constexpr auto make(T &&source) noexcept(true) {
    using parent_::parent_::parent_::parent_::generator::function::make;
    return make(this_::driver::make(::std::forward<T>(source)));
}

} // namespace normailzer

template <class T> inline constexpr auto make(T &&driver) noexcept(true) {
    return this_::Type{::std::forward<T>(driver)};
}

} // namespace p5::lambda::utils::log::message::text
