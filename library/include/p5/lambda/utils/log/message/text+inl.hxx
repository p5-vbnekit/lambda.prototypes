#pragma once

#include <tuple>
#include <ranges>
#include <string>
#include <utility>
#include <string_view>
#include <type_traits>

#include "../../generator.hxx"

#include "text.hxx"


namespace p5::lambda::utils::log::message::text {
namespace chunk {
namespace private_ {

namespace parent_ = this_;
namespace this_ = parent_::private_;

inline static Type make() noexcept(true) { return {}; }

inline static Type make(char const *source) noexcept(true) {
    if (source) return {.reference = source, .terminated = true};
    return {};
}

inline static Type make(::std::string const &source) noexcept(true) {
    if (source.empty()) return {.terminated = false};
    return {.reference = source.c_str(), .terminated = true};
}

inline static Type make(
    ::std::string_view &&source, bool terminated = false
) noexcept(true) {
    return {.reference = ::std::move(source), .terminated = terminated};
}

inline static Type make(
    ::std::string_view const &source, bool terminated = false
) noexcept(true) {
    return {.reference = source, .terminated = terminated};
}

} // namespace private_

template <class ... T> inline constexpr static auto
make(T && ... source) noexcept(true) requires([] {
    return requires () { private_::make(::std::declval<T>() ...); };
} ()) {
    return this_::private_::make(::std::forward<T>(source) ...);
}

} // namespace chunk

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

namespace private_ {

namespace parent_ = this_;
namespace this_ = private_;

namespace emplace {

namespace parent_ = this_;
namespace this_ = emplace;

template <class T> inline constexpr static
auto ensure_chunk_(T &&source) noexcept(true) requires([] () {
    return requires () { parent_::parent_::chunk::make(::std::declval<T>()); };
} ()) {
    if constexpr (
        ::std::is_same_v<T, parent_::parent_::Chunk>
    ) return ::std::forward<T>(source);
    else return parent_::parent_::chunk::make(::std::forward<T>(source));
}

namespace introspection_ {

namespace parent_ = this_;
namespace this_ = introspection_;

namespace sfinae {

namespace parent_ = this_;
namespace this_ = sfinae;

template <class, class = void> struct EnsureChunk final: ::std::false_type {};
template <class T> struct EnsureChunk<T, ::std::void_t<
    decltype(parent_::parent_::ensure_chunk_(::std::declval<T>()))
>> final: ::std::true_type {};

template <class T> inline constexpr static auto ensure_chunk() noexcept(true) {
    return this_::EnsureChunk<T>::value;
}

} // namespace sfinae

template <class = void> struct IsTuple final: ::std::false_type {};

template <class ... T> struct IsTuple<
    ::std::tuple<T ...>
> final: ::std::true_type {};

template <class T> using RemoveRvalue = ::std::conditional_t<
    ::std::is_rvalue_reference_v<T>,
    ::std::remove_reference_t<T>, T
>;

template <class T> inline constexpr static
auto is_tuple() noexcept(true) { return this_::IsTuple<T>::value; }

} // namespace introspection_

template <class Destination> inline static
auto implementation(Destination &) noexcept(true) {}

template <class D, class F, class ... T> inline static
void implementation(D &destination, F &&first, T && ... tail) noexcept(false) {
    using First_ = ::std::decay_t<F>;
    if constexpr (
        this_::introspection_::is_tuple<First_>()
    ) ::std::apply([&destination] (auto && ... values) {
        implementation(destination, ::std::forward<
            this_::introspection_::RemoveRvalue<decltype(values)>
        >(values) ...);
    }, ::std::forward<F>(first));
    else if constexpr (
        this_::introspection_::sfinae::ensure_chunk<First_>()
    ) destination.emplace_back(this_::ensure_chunk_(::std::forward<F>(first)));
    else {
#if ! (defined(__clang_major__) && (16 > __clang_major__))
        static_assert(requires () {
            ::std::views::all(::std::forward<F>(first));
        });
#endif
        for (auto &&item_: ::std::forward<F>(first)) {
            implementation(destination, ::std::forward<
                this_::introspection_::RemoveRvalue<decltype(item_)>
            >(item_));
        }
    }
    implementation(destination, ::std::forward<T>(tail) ...);
}

} // namespace emplace
} // namespace private_

template <class T> inline auto this_::Type::view() const noexcept(true) {
    static_assert(::std::is_same_v<T, void>);
    using parent_::parent_::parent_::generator::function::make;
#if ! (defined(__clang_major__) && (16 > __clang_major__))
    return ::std::views::all(make([
#else
    return (make([
#endif
        end_ = chunks_.end(), iterator_ = chunks_.begin()
    ] () mutable {
        using Pointer_ = ::std::decay_t<decltype(*iterator_)> const *;
        if (end_ == iterator_) return static_cast<Pointer_>(nullptr);
        return &(*(iterator_++));
    }));
}

template <class ... T> auto this_::Type::emplace(T && ... source) {
    this_::private_::emplace::implementation(
        chunks_, ::std::forward<T>(source) ...
    );
}

inline auto this_::Type::operator * () const noexcept(true) { return view(); }

template <class ... T> inline static
auto make(T && ... source) noexcept(true) {
    return this_::Type{::std::forward<T>(source) ...};
}

} // namespace p5::lambda::utils::log::message::text
