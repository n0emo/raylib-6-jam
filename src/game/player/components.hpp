#pragma once

#include <entt/entt.hpp>

namespace game::components {

struct Player {};

auto create_player(entt::registry& registry) -> entt::entity;

} // namespace game::components
