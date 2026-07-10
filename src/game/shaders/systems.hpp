#pragma once

#include <entt/fwd.hpp>

namespace cfu::systems {

auto setup_shaders(entt::registry& registry) -> void;

auto reload_shaders(entt::registry& registry) -> void;
auto reload_default_shader(entt::registry& registry) -> void;
auto reload_depth_shader(entt::registry& registry) -> void;
auto reload_lighting_shader(entt::registry& registry) -> void;

auto update_shaders(entt::registry& registry) -> void;
auto update_lighting_shader(entt::registry& registry) -> void;
auto update_shadow_map(entt::registry& registry) -> void;

auto draw_shaders(entt::registry& registry) -> void;

}; // namespace cfu::systems
