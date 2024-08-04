#pragma once

#include <tuple>
#include <memory>
#include <variant>
#include <utility>
#include <stdexcept>
#include <functional>
#include <type_traits>

#include "lock.hxx"


namespace p5::lambda::utils::container::lock {
namespace driver {
namespace holder_ {

namespace parent_ = this_;
namespace this_ = parent_::holder_;

template <class = void> struct Type;

template <> struct Type<void> {
    void * const pointer;

    virtual ~Type() noexcept(true);

protected:
    inline Type(void *pointer = nullptr) noexcept(true): pointer{pointer} {}

private:
    Type(Type &&) = delete;
    Type(Type const &) = delete;
    Type & operator = (Type &&) = delete;
    Type & operator = (Type const &) = delete;
};

template <class Instance> struct Type final: Type<void> {
    static_assert(! ::std::is_reference_v<Instance>);
    static_assert(! ::std::is_pointer_v<Instance>);

    template <class ... T> inline Type(T && ... payload) requires([] {
        if constexpr (1 != sizeof ... (T)) return true;
        else {
            using Type_ = ::std::tuple_element_t<
                0, ::std::tuple<::std::decay_t<T> ...>
            >;
            return ! ::std::is_base_of_v<Type<Instance>, Type_>;
        }
    } ()): Type<void>{[this] {
        if constexpr (
            ::std::is_const_v<Instance>
        ) return const_cast<Instance *>(&instance_);
        else return &instance_;
    } ()}, instance_{::std::forward<T>(payload) ...} {}

private:
    Instance instance_;
};

} // namespace holder_

template <class T> using Holder_ = holder_::Type<T>;

struct Type {
    using Holder = this_::Holder_<void>;

    virtual void put(::std::variant<
        void *, ::std::unique_ptr<Holder>,
        ::std::function<void *(void)>,
        ::std::function<::std::unique_ptr<Holder>(void)>
    > &&) noexcept(false) = 0;

    virtual void take() noexcept(true) = 0;

    virtual ~Type() noexcept(true);

protected:
    Type() noexcept(true) = default;
    Type(Type &&) noexcept(true) = default;
    Type & operator = (Type &&) noexcept(true) = default;

private:
    Type(Type const &) = delete;
    Type & operator = (Type const &) = delete;
};

} // namespace driver

template <class Interface> inline
auto && this_::Type<Interface>::take() noexcept(true) {
    if (driver_) driver_->take();
    return ::std::move(*this);
}

template <class Interface> inline
auto this_::Type<Interface>::release() noexcept(true) {
    if (! driver_) return;
    driver_->take();
    driver_.reset();
}

template <class Interface> template <class Factory> inline
auto this_::Type<Interface>::inject(Factory &&factory) noexcept(false) {
    if (! driver_) throw ::std::logic_error{"expired"};
    using Result_ = decltype(factory());
    if constexpr ([&factory] {
        if constexpr (::std::is_pointer_v<Result_>) return true;
        else return ::std::is_lvalue_reference_v<Result_>;
    }) return driver_->put([factory_ = ::std::forward<Factory>(factory)] {
        return static_cast<void *>([pointer_ = [&factory_] {
            auto &&value_ = factory_();
            if constexpr (::std::is_pointer_v<
                ::std::decay_t<Result_>
            >) return value_;
            else return &value_;
        } ()] {
            if constexpr (
                ::std::is_const_v<Interface>
            ) return const_cast<Interface *>(pointer_);
            else return pointer_;
        } ());
    });

    else return driver_->put([factory_ = ::std::forward<Factory>(factory)] {
        return ::std::make_unique<
            this_::driver::Holder_<::std::remove_reference_t<Result_>>
        >(factory_());
    });
}

template <class Interface> template <class Instance> inline
auto this_::Type<Interface>::assign(Instance &instance) noexcept(false) {
    if (! driver_) throw ::std::logic_error{"expired"};
    driver_->put([pointer_ = [&instance] {
        if constexpr (::std::is_pointer_v<Instance>) return instance;
        else return &instance;
    } ()] {
        if constexpr (
            ::std::is_const_v<Interface>
        ) return const_cast<Interface *>(pointer_);
        else return pointer_;
    } ());
}

template <class Interface> template <class Instance, class ... T> inline
auto this_::Type<Interface>::emplace(T && ... payload) noexcept(false) {
    static_assert(! ::std::is_pointer_v<Instance>);
    static_assert(! ::std::is_reference_v<Instance>);
    if (! driver_) throw ::std::logic_error{"expired"};
    driver_->put(::std::make_unique<this_::driver::Holder_<Instance>>(
        ::std::forward<T>(payload) ...
    ));
}

template <class Interface> inline
this_::Type<Interface>::Type(::std::unique_ptr<Driver> &&driver) noexcept(true):
    driver_{::std::move(driver)}
{}

} // namespace p5::lambda::utils::container::lock
