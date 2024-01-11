#pragma once

#include <type_traits>

#include <p5/lambda.hxx>


namespace p5::lambda::metamod_plugin {

namespace parent_ = this_;
namespace this_ = parent_::metamod_plugin;

struct GameFunctionsPointers final {
    using Standard = parent_::game::functions::Standard;
    using Extension = parent_::game::functions::Extension;

    Standard const *standard = nullptr;
    Extension const *extension = nullptr;
};

struct Singleton final : private parent_::utils::sugar::Uncopyable {
    struct Engine final {
        using Globals = parent_::engine::Globals;
        using Functions = parent_::engine::Functions;

        Functions functions = {};
        Globals *globals = nullptr;
    };

    struct Game final {
        using Functions = parent_::game::Functions;

        Functions functions = {};
    };

    struct Meta final {
        using Globals = parent_::metamod::Globals;
        using Functions = parent_::metamod::Functions;

        struct Plugin {
            using Info = parent_::metamod::plugin::Info;
            using Functions = parent_::metamod::plugin::Functions;

            Info info {
                .name = P5_LAMBDA_METAMOD_PLUGIN_NAME,
                .version = P5_LAMBDA_METAMOD_PLUGIN_VERSION,
                .date = P5_LAMBDA_METAMOD_PLUGIN_TIMESTAMP,
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
//    Test test;

//    template <Logs::Level V = Logs::Level::Info, class ... T> inline auto log(T && ... options) const noexcept(true) {
//        return mLogs.make<V>(::std::forward<T>(options) ...);
//    }

    inline static auto & instance() noexcept(true) { static Singleton instance; return instance; }

private:
//    Logs mLogs {makeLogsOptions()};

//    inline Logs::Options makeLogsOptions() const noexcept(true) {
//        return {
//            meta.plugin.info.logtag,
//            [] () { return Logs::Level::Error; },
//            {},
//            {}
//        };
//    }

    Singleton() = default;
};

} // namespace p5::lambda::metamod_plugin
