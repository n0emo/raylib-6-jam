#pragma once

#include <entt/entity/registry.hpp>

namespace cfu::systems {

auto setup_physics(entt::registry& registry) -> void;
auto update_physics(entt::registry& registry) -> void;
auto update_physics_velocity(entt::registry& registry) -> void;
auto draw_physics(entt::registry& registry) -> void;
auto draw_physics_velocity(entt::registry& registry) -> void;
auto draw_physics_circle_colliders(entt::registry& registry) -> void;

} // namespace cfu::systems
