#pragma once

#include <raylib.h>
#include <entt/entt.hpp>
#include <imgui_entt_entity_editor.hpp>

namespace cfu::components {

struct BackgroundColor {
    Color color;
};

} // namespace cfu::components

namespace MM {

template<>
void ComponentEditorWidget<cfu::components::BackgroundColor>(entt::registry& reg, entt::registry::entity_type e);

} // namespace MM
