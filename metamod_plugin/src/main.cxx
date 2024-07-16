#include <cstdint>

#include <string>
#include <utility>
#include <sstream>
#include <exception>
#include <stdexcept>

#include <boost/algorithm/string.hpp>

#include <p5/lambda.hxx>

#include "main.hxx"
#include "singleton.hxx"


namespace root_::main {
namespace private_ {

namespace parent_ = this_;
namespace this_ = parent_::private_;

namespace lambda = ::p5::lambda;
namespace plugin = this_::lambda::metamod_plugin;

using Log = plugin::Singleton::Log;

template <class T> inline static
auto exception_details_(T &&exception) noexcept(true) {
    namespace utils_ = this_::lambda::utils;
    namespace chunk_ = utils_::log::message::text::chunk;

    using Chunk_ = ::std::decay_t<decltype(chunk_::make())>;

    struct State_ final {
        ::std::optional<Chunk_> value = ::std::nullopt;
        ::std::string buffer = {};
        ::std::exception_ptr pointer;
    };

    return utils_::generator::function::make([
        state_ = State_{.pointer = [&exception] {
            if constexpr (::std::is_base_of_v<
                ::std::exception, ::std::decay_t<T>
            >) return ::std::make_exception_ptr(::std::forward<T>(exception));
            else return ::std::forward<T>(exception);
        } ()}
    ] () mutable -> Chunk_ const * {
        if (state_.pointer) try {
            if (state_.value) {
                thread_local static auto const new_line_ = chunk_::make("\n");
                state_.value.reset();
                return &new_line_;
            }
            namespace exception_ = utils_::exception;
            state_.buffer = exception_::details(state_.pointer);
            state_.pointer = exception_::next(state_.pointer);
            ::boost::algorithm::trim(state_.buffer);
            if (state_.buffer.empty()) {
                thread_local static auto const unknown_ = chunk_::make(
                    "Unknown exception."
                );
                state_.value.emplace(unknown_);
            } else state_.value.emplace(chunk_::make(state_.buffer));
            return &(*(state_.value));
        }
        catch (...) { state_.pointer = {}; }
        return nullptr;
    });
}

inline static auto exception_details_() noexcept(true) {
    return this_::exception_details_(::std::current_exception());
}

} // namespace private_

// `GiveFnptrsToDll` - receive engine function table from engine.
// This appears to be the _first_ DLL routine called by the engine, so we do some setup operations here.
void this_::game::inject(
    parent_::engine::Functions const *functions,
    parent_::engine::Globals *globals
) noexcept(true) {
    namespace private_ = parent_::private_;
    auto &singleton_ = private_::plugin::Singleton::instance();

    try {
        singleton_.log.write<
            private_::Log::Message::Level::Developer
        >() << "globals = " << static_cast<void const *>(globals)
        << ", functions = " << functions;

        if (! static_cast<bool>(globals)) throw ::std::invalid_argument{
            "globals pointer is null"
        };

        if (! static_cast<bool>(functions)) throw ::std::invalid_argument{
            "functions pointer is null"
        };

        singleton_.engine.globals = globals;
        singleton_.engine.functions = *functions;

        singleton_.log.write<
            private_::Log::Message::Level::Developer
        >() << "success";
    }

    catch(...) { singleton_.log.write(
        private_::Log::Message::Level::Error,
        private_::exception_details_()
    ); }
}

// `Meta_Query` - metamod requesting info about this plugin:
// - `version`      (given)     interface_version metamod is using
// - `info`         (requested) struct with info about plugin
// - `functions`    (given)     table of utility functions provided by metamod
this_::Boolean this_::metamod::plugin::query(
    char const *version, this_::Info **info, parent_::Functions const *functions
) noexcept(true) {
    namespace private_ = parent_::parent_::private_;
    auto &singleton_ = private_::plugin::Singleton::instance();

    try {
        auto &&version_ = ::std::string{
            singleton_.meta.plugin.info.interface_version
        };

        singleton_.log.write<
            private_::Log::Message::Level::Developer
        >() << "interface version = \"" << version_
        << "\", plugin info = " << static_cast<void const *>(info)
        << ", util functions = " << static_cast<void const *>(functions);

        if (! static_cast<bool>(info)) throw ::std::invalid_argument{
            "plugin info pointer is null"
        };

        *info = &singleton_.meta.plugin.info;

        if (! (static_cast<bool>(version) && (
            singleton_.meta.plugin.info.interface_version == version_
        ))) {
            ::std::ostringstream stream_;
            stream_ << "interface version mismatch: plugin["
                << singleton_.meta.plugin.info.interface_version
                << "] != metamod[" << ::std::move(version_)
            << "]";
            throw ::std::invalid_argument{stream_.str()};
        }

        if (! static_cast<bool>(functions)) throw ::std::invalid_argument{
            "util functions pointer is null"
        };

        singleton_.meta.functions = *functions;

        singleton_.log.write<
            private_::Log::Message::Level::Developer
        >() << "success";

        return Boolean::True;
    }

    catch(...) { singleton_.log.write(
        private_::Log::Message::Level::Error,
        private_::exception_details_()
    ); }

    return Boolean::False;
}

// `Meta_Attach` - metamod attaching plugin to the server:
// - `phase`            (given)     current phase, ie during map, during changelevel, or at startup
// - `functions`        (requested) table of function tables this plugin catches
// - `globals`          (given)     global vars from metamod
// - `game_functions`   (given)     copy of function tables from game shared module
this_::Boolean this_::metamod::plugin::attach(
    this_::Phase, this_::Functions *functions, parent_::Globals *globals,
    parent_::parent_::game::functions::Pointers const *game_functions
) noexcept(true) {
    namespace private_ = parent_::parent_::private_;
    auto &singleton_ = private_::plugin::Singleton::instance();

    try {
        try {
            singleton_.log.write<
                private_::Log::Message::Level::Developer
            >() << "phase = " << static_cast<::std::size_t>(phase)
            << ", plugin functions = " << static_cast<void const *>(functions)
            << ", globals = " << static_cast<void const *>(globals)
            << ", game functions = " << static_cast<void const *>(
                game_functions
            );
        } catch (...) {}

        if (! static_cast<bool>(
            singleton_.engine.globals
        )) throw ::std::runtime_error{"GiveFnptrsToDll failed"};

        if (! static_cast<bool>(
            functions
        )) throw ::std::invalid_argument{"plugin functions pointer is null"};

        if (! static_cast<bool>(
            game_functions
        )) throw ::std::invalid_argument{"game functions pointer is null"};

        auto const * const standard = game_functions->standard;
        if (! static_cast<bool>(standard)) throw ::std::invalid_argument{
            "standard game functions pointer is null"
        };

        auto const * const extension = game_functions->extension;
        if (! static_cast<bool>(extension)) throw ::std::invalid_argument{
            "\new\" game functions pointer is null"
        };

        *functions = singleton_.meta.plugin.functions;

        singleton_.meta.globals = globals;
        singleton_.game.functions.standard = *standard;
        singleton_.game.functions.extension = *extension;

        singleton_.log.write<
            private_::Log::Message::Level::Developer
        >() << "success";

        return Boolean::True;
    }

    catch(...) { singleton_.log.write(
        private_::Log::Message::Level::Error,
        private_::exception_details_()
    ); }

    return Boolean::False;
}

// `Meta_Detach` - metamod detaching plugin from the server:
// - `phase`    (given) current phase, ie during map, etc
// - `reason`   (given) why detaching (refresh, console unload, forced unload, etc)
this_::Boolean this_::metamod::plugin::detach(
    this_::Phase phase, this_::UnloadReason reason
) noexcept(true) {
    namespace private_ = parent_::parent_::private_;
    auto &singleton_ = private_::plugin::Singleton::instance();

    try {
        try {
            singleton_.log.write<
                private_::Log::Message::Level::Developer
            >() << "phase = " << static_cast<::std::size_t>(phase)
            << ", reason = " << static_cast<::std::size_t>(reason);
        } catch (...) {}

        singleton_.meta.globals = nullptr;
        singleton_.game.functions = {};

        singleton_.log.write<
            private_::Log::Message::Level::Developer
        >() << "success";

        return Boolean::True;
    }

    catch(...) { singleton_.log.write(
        private_::Log::Message::Level::Error,
        private_::exception_details_()
    ); }

    return Boolean::False;
}

} // namespace root_::main
