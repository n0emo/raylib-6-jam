#pragma once

#include <entt/fwd.hpp>
#include <raylib.h>

namespace cfu::systems {

// TODO: reload_fonts

auto setup_font_cache(entt::registry& registry) -> void;
auto reload_fonts(entt::registry& registry) -> void;
} // namespace cfu::systems
