#include "singleton.hxx"


namespace p5::lambda::metamod_plugin::singleton {

this_::Type & this_::Type::instance() noexcept(true) {
    static auto instance_ = Type{}; return instance_;
}

this_::Type::Type() noexcept(true) : log_{{
    .meta = {.api = meta.functions, .plugin = meta.plugin.info},
    .engine = engine.functions
}} {}

} // namespace p5::lambda::metamod_plugin::singleton
