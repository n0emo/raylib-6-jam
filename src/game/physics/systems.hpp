#pragma once

#include <entt/entt.hpp>

namespace game::systems {

auto setup_physics(entt::registry& registry) -> void;
auto update_physics(entt::registry& registry) -> void;

} // namespace game::systems
