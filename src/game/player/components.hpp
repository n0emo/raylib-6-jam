#pragma once

#include <entt/entt.hpp>

namespace cfu::components {

struct Player {};

auto create_player(entt::registry& registry, entt::entity entity) -> void;

} // namespace cfu::components
