#pragma once

#include <p5/lambda.hxx>

#include "namespace_.hxx"


namespace p5::lambda::metamod_plugin {
namespace singleton {

namespace parent_ = this_;
namespace this_ = parent_::singleton;

struct Engine final {
    using Globals = parent_::parent_::engine::Globals;
    using Functions = parent_::parent_::engine::Functions;

    Functions functions = {};
    Globals *globals = nullptr;
};

struct Game final {
    using Functions = parent_::parent_::game::Functions;

    Functions functions = {};
};

struct Meta final {
    using Globals = parent_::parent_::metamod::Globals;
    using Functions = parent_::parent_::metamod::Functions;

    struct Plugin final {
        using Info = parent_::parent_::metamod::plugin::Info;
        using Functions = parent_::parent_::metamod::plugin::Functions;

        Info info = {
#ifdef P5_LAMBDA_METAMOD_PLUGIN_NAME
            .name = P5_LAMBDA_METAMOD_PLUGIN_NAME,
#endif
#ifdef P5_LAMBDA_METAMOD_PLUGIN_VERSION
            .version = P5_LAMBDA_METAMOD_PLUGIN_VERSION,
#endif
#ifdef P5_LAMBDA_METAMOD_PLUGIN_TIMESTAMP
            .date = P5_LAMBDA_METAMOD_PLUGIN_TIMESTAMP,
#endif
            .author = "p5-vbnekit <vbnekit@gmail.com>",
            .url = "https://github.com/p5-vbnekit/lambda.prototypes",
            .log_tag = "P5-LAMBDA"
        };

        // Functions functions = hooks::make();
        Functions functions = {};
    };

    Plugin plugin = {};
    Functions functions = {};
    Globals *globals = nullptr;
};

using Container = parent_::parent_::utils::Container;

struct Type final : private parent_::parent_::utils::sugar::Uncopyable {
    using Container = this_::Container;
    using Engine = this_::Engine;
    using Game = this_::Game;
    using Meta = this_::Meta;

    Container container = {};
    Engine engine = {};
    Game game = {};
    Meta meta = {};

    static Type & instance() noexcept(false);

private:
    parent_::parent_::utils::event::Subscription on_error_;

    Type() noexcept(false);

    Type(Type &&) = delete;
    Type(Type const &) = delete;
    Type & operator = (Type &&) = delete;
    Type & operator = (Type const &) = delete;
};

} // namespace singleton

using Singleton = singleton::Type;

} // namespace p5::lambda::metamod_plugin
