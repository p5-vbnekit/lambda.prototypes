#pragma once

#include <p5/lambda/game+fwd.hxx>
#include <p5/lambda/engine+fwd.hxx>
#include <p5/lambda/common+fwd.hxx>
#include <p5/lambda/metamod+fwd.hxx>

#include "root_.hxx"


namespace root_::main {

namespace parent_ = this_;
namespace this_ = parent_::main;

namespace engine {

namespace parent_ = this_;
namespace this_ = parent_::engine;

using Functions = ::p5::lambda::engine::Functions;
using Globals = ::p5::lambda::engine::Globals;

} // namespace engine

namespace game {

namespace parent_ = this_;
namespace this_ = parent_::game;

void inject(
    parent_::engine::Functions const *, parent_::engine::Globals *
) noexcept(true);

namespace functions {

namespace parent_ = this_;
namespace this_ = parent_::functions;

using Pointers = ::p5::lambda::game::functions::Pointers;

} // namespace functions
} // namespace game

using Boolean = ::p5::lambda::common::IntegerBoolean;

namespace metamod {

namespace parent_ = this_;
namespace this_ = parent_::metamod;

using Globals = ::p5::lambda::metamod::Globals;
using Functions = ::p5::lambda::metamod::Functions;

namespace plugin {

namespace parent_ = this_;
namespace this_ = parent_::plugin;

using Info = ::p5::lambda::metamod::plugin::Info;
using Phase = ::p5::lambda::metamod::plugin::LoadTime;
using Functions = ::p5::lambda::metamod::plugin::Functions;
using UnloadReason = ::p5::lambda::metamod::plugin::UnloadReason;

parent_::parent_::Boolean query(
    char const *, this_::Info **, parent_::Functions const *
) noexcept(true);

parent_::parent_::Boolean attach(
    this_::Phase, this_::Functions *, parent_::Globals *,
    parent_::parent_::game::functions::Pointers const *
) noexcept(true);

parent_::parent_::Boolean detach(
    this_::Phase, this_::UnloadReason
) noexcept(true);

} // namespace plugin
} // namespace metamod
} // namespace root_::main
