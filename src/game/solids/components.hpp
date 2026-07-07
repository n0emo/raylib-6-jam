#pragma once

#include <raylib.h>
#include <entt/entt.hpp>
#include <imgui_entt_entity_editor.hpp>

namespace cfu::components {

struct SolidMaterial {
    Color color;
};

struct Cube {
    Vector3 size;
};

} // namespace cfu::components

namespace MM {

template<>
void ComponentEditorWidget<cfu::components::SolidMaterial>(entt::registry& reg, entt::registry::entity_type e);

template<>
void ComponentEditorWidget<cfu::components::Cube>(entt::registry& reg, entt::registry::entity_type e);

} // namespace MM
