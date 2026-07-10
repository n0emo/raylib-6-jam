#pragma once

#include <entt/fwd.hpp>

namespace cfu::comp {

struct Enemy {};

struct Follows {
    entt::entity entity;
};

auto create_enemy(entt::registry& reg, entt::entity e) -> void;

} // namespace cfu::comp
