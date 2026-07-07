#pragma once

#include <entt/entt.hpp>
#include <imgui_entt_entity_editor.hpp>
#include <raylib.h>

namespace cfu::components {

struct ShapeColor {
    Color color;
};

struct Ball {
    float radius;
};

} // namespace cfu::components

namespace MM {

template<>
void ComponentEditorWidget<cfu::components::ShapeColor>(entt::registry& reg, entt::registry::entity_type e);

template<>
void ComponentEditorWidget<cfu::components::Ball>(entt::registry& reg, entt::registry::entity_type e);

} // namespace MM
