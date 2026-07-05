#pragma once

#include <entt/entt.hpp>
#include <imgui_entt_entity_editor.hpp>
#include <raylib.h>

namespace game::components {

struct ShapeColor {
    Color color;
};

struct Ball {
    float radius;
};

} // namespace game::components

namespace MM {

template<>
void ComponentEditorWidget<game::components::ShapeColor>(entt::registry& reg, entt::registry::entity_type e);

template<>
void ComponentEditorWidget<game::components::Ball>(entt::registry& reg, entt::registry::entity_type e);

} // namespace MM
