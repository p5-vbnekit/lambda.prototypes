#pragma once

#include <p5/lambda.hxx>

#include "namespace_.hxx"


namespace p5::lambda::metamod_plugin::api {

namespace parent_ = this_;
namespace this_ = parent_::api;

using Game = parent_::parent_::game::Functions;

struct Engine final {
    using Globals = parent_::parent_::engine::Globals;
    using Functions = parent_::parent_::engine::Functions;

    Functions functions = {};
    Globals *globals = nullptr;
};

} // namespace p5::lambda::metamod_plugin::api
