#pragma once

#include <type_traits>

#include "text+fwd.hxx"

#include "text/type.hxx"
#include "text/chunk.hxx"
#include "text/driver.hxx"


namespace p5::lambda::utils::log::message::text {
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

namespace normailzer {
namespace driver {

template <> struct Type<this_::Interface *> final: this_::Interface {
    using Chunk = this_::Chunk;
    using Source = this_::Interface *;

    virtual Chunk const * operator () () noexcept(true) override final;

    explicit Type(Source = nullptr) noexcept(true);
    Type(Type &&) noexcept(true);

private:
    Source source_;

    Type(Type const &) = delete;
    Type & operator = (Type &&) = delete;
    Type & operator = (Type const &) = delete;
};

template <class T> struct Type final: this_::Interface {
    using Chunk = this_::Chunk;
    using Source = ::std::remove_reference_t<T>;

    static_assert(::std::is_same_v<Source, T>);

    virtual Chunk const * operator () () noexcept(true) override final;

    explicit Type(Source &&) noexcept(true);
    virtual ~Type() noexcept(true) override final;

private:
    struct Adapter_ final: this_::Interface {
        virtual Chunk const * operator () () noexcept(true) override final;

        explicit Adapter_(Source &&) noexcept(true);
        virtual ~Adapter_() noexcept(true) override final;

    private:
        Source source_;

        Adapter_(Type const &) = delete;
        Adapter_ & operator = (Adapter_ &&) = delete;
        Adapter_ & operator = (Adapter_ const &) = delete;
    };

    Adapter_ adapter_;
    this_::Type<this_::Interface *> implementation_{&adapter_};

    Type(Type const &) = delete;
    Type & operator = (Type &&) = delete;
    Type & operator = (Type const &) = delete;
};

} // namespace driver
} // namespace normailzer

} // namespace p5::lambda::utils::log::message::text


#include "text+inl.hxx"
