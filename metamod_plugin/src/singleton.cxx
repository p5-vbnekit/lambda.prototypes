#include <memory>
#include <utility>
#include <variant>
#include <sstream>
#include <iostream>
#include <exception>
#include <stdexcept>
#include <type_traits>

#include <p5/lambda.hxx>

#include <boost/core/demangle.hpp>
#include <boost/algorithm/string.hpp>

#include "singleton.hxx"


namespace p5::lambda::metamod_plugin::singleton {

this_::Type & this_::Type::instance() noexcept(false) {
    static auto holder_ = ::std::variant<
        ::std::unique_ptr<Type>, ::std::exception_ptr
    >{::std::in_place_index_t<0>{}};

    if (0 == holder_.index()) {
        auto &pointer_ = ::std::get<0>(holder_);
        if (! pointer_) {
            try { pointer_.reset(new ::std::decay_t<decltype(*pointer_)>); }
            catch (...) {
                holder_.emplace<1>(::std::current_exception()); throw;
            }
        }
        return *pointer_;
    }

    if (1 == holder_.index()) {
        auto const &exception_ = ::std::get<1>(holder_);
        if (exception_) ::std::rethrow_exception(exception_);
    }

    throw ::std::logic_error{"invalid state"};
}

this_::Type::Type() noexcept(false): on_error_{container.on_error([] (
    auto &&key, auto &&exception
) {
    auto key_ = ::boost::core::demangle(key.name());
    auto exception_ = parent_::parent_::utils::exception::details(exception);
    ::boost::algorithm::trim(exception_);
    ::std::ostringstream stream_;
    if (exception_.empty()) {
        stream_ << "unknown container error";
        if (! key_.empty()) stream_ << " [" << ::std::move(key_) << "]";
    }
    else {
        stream_ << "container error";
        if (! key_.empty()) stream_ << " [" << ::std::move(key_) << "]";
        stream_ << ": " << ::std::move(exception_);
    }
    ::std::cerr << stream_.view() << ::std::endl << ::std::flush;
})} {}

} // namespace p5::lambda::metamod_plugin::singleton
