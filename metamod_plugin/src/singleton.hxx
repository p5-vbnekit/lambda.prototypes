#pragma once

#include <p5/lambda.hxx>

#include "namespace_.hxx"


namespace p5::lambda::metamod_plugin {
namespace singleton {

namespace parent_ = this_;
namespace this_ = parent_::singleton;

struct Type final : private parent_::parent_::utils::sugar::Uncopyable {
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

    Engine engine = {};
    Game game = {};
    Meta meta = {};

    inline static auto & instance() noexcept(true) {
        static Type instance; return instance;
    }

private:
    Type() = default;
};

} // namespace singleton

using Singleton = singleton::Type;

} // namespace p5::lambda::metamod_plugin
