#pragma once

#include <entt/entt.hpp>

namespace game::systems {

auto setup_dev(entt::registry& registry) -> void;
auto draw_dev(entt::registry& registry) -> void;
auto update_dev(entt::registry& registry) -> void;

auto draw_dev_panel(entt::registry& registry) -> void;

auto setup_entt_editor(entt::registry& registry) -> void;
auto draw_entt_editor(entt::registry& registry) -> void;

} // namespace game::systems
