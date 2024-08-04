#include <cstdint>

#include <string>
#include <utility>
#include <sstream>
#include <stdexcept>

#include <p5/lambda.hxx>

#include "main.hxx"
#include "singleton.hxx"


namespace root_::main {

using Singleton_ = ::p5::lambda::metamod_plugin::Singleton;

// `GiveFnptrsToDll` - receive engine function table from engine.
// This appears to be the _first_ DLL routine called by the engine, so we do some setup operations here.
void this_::game::inject(
    parent_::engine::Functions const *functions,
    parent_::engine::Globals *globals
) noexcept(true) {
    auto &singleton_ = parent_::Singleton_::instance();

    try {
        if (! static_cast<bool>(globals)) throw ::std::invalid_argument{
            "globals pointer is null"
        };

        if (! static_cast<bool>(functions)) throw ::std::invalid_argument{
            "functions pointer is null"
        };

        singleton_.engine.globals = globals;
        singleton_.engine.functions = *functions;

        singleton_.container.assign(singleton_.engine.globals);
        singleton_.container.assign(singleton_.engine.functions);
    }

    catch(...) {}
}

// `Meta_Query` - metamod requesting info about this plugin:
// - `version`      (given)     interface_version metamod is using
// - `info`         (requested) struct with info about plugin
// - `functions`    (given)     table of utility functions provided by metamod
this_::Boolean this_::metamod::plugin::query(
    char const *version, this_::Info **info, parent_::Functions const *functions
) noexcept(true) {
    auto &singleton_ = parent_::parent_::Singleton_::instance();

    try {
        auto &&version_ = ::std::string{
            singleton_.meta.plugin.info.interface_version
        };

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

        return Boolean::True;
    }

    catch(...) {}

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
    auto &singleton_ = parent_::parent_::Singleton_::instance();

    try {
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

        return Boolean::True;
    }

    catch(...) {}

    return Boolean::False;
}

// `Meta_Detach` - metamod detaching plugin from the server:
// - `phase`    (given) current phase, ie during map, etc
// - `reason`   (given) why detaching (refresh, console unload, forced unload, etc)
this_::Boolean this_::metamod::plugin::detach(
    this_::Phase, this_::UnloadReason
) noexcept(true) {
    auto &singleton_ = Singleton_::instance();

    try {
        singleton_.meta.globals = nullptr;
        singleton_.game.functions = {};
        return Boolean::True;
    }

    catch(...) {}

    return Boolean::False;
}

} // namespace root_::main
