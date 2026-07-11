#pragma once

#include <entt/entity/registry.hpp>

#include "./resource.hpp"

namespace cfu::systems {

auto setup_level_cache(entt::registry& registry) -> void;
auto reload_levels(entt::registry& registry) -> void;
auto save_levels(entt::registry& registry) -> void;
auto spawn_level(entt::registry& registry, res::level::LevelId id) -> void;
auto update_level(entt::registry& registry) -> void;

} // namespace cfu::systems
