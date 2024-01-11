#include "global.h"
#include "main.hxx"


P5_LAMBDA_METAMOD_PLUGIN_GLOBAL_EXTERN_C ::root_::main::Boolean
P5_LAMBDA_METAMOD_PLUGIN_GLOBAL_EXPORT Meta_Query(
    char const *version,
    ::root_::main::metamod::plugin::Info **info,
    ::root_::main::metamod::Functions const *functions
) {
    return ::root_::main::metamod::plugin::query(version, info, functions);
}

P5_LAMBDA_METAMOD_PLUGIN_GLOBAL_EXTERN_C ::root_::main::Boolean
P5_LAMBDA_METAMOD_PLUGIN_GLOBAL_EXPORT Meta_Attach(
    ::root_::main::metamod::plugin::Phase phase,
    ::root_::main::metamod::plugin::Functions *functions,
    ::root_::main::metamod::Globals *globals,
    ::root_::main::game::functions::Pointers const *game
) {
    return ::root_::main::metamod::plugin::attach(
        phase, functions, globals, game
    );
}

P5_LAMBDA_METAMOD_PLUGIN_GLOBAL_EXTERN_C ::root_::main::Boolean
P5_LAMBDA_METAMOD_PLUGIN_GLOBAL_EXPORT Meta_Detach(
    ::root_::main::metamod::plugin::Phase phase,
    ::root_::main::metamod::plugin::UnloadReason reason
) {
    return ::root_::main::metamod::plugin::detach(phase, reason);
}

#ifdef _WIN32
#include <windows.h>
#endif // _WIN32

P5_LAMBDA_METAMOD_PLUGIN_GLOBAL_EXTERN_C void
P5_LAMBDA_METAMOD_PLUGIN_GLOBAL_EXPORT
P5_LAMBDA_METAMOD_PLUGIN_GLOBAL_WINAPI
GiveFnptrsToDll(
    ::root_::main::engine::Functions const *functions,
    ::root_::main::engine::Globals *globals
) {
    return ::root_::main::game::inject(functions, globals);
}
