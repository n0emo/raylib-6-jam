#pragma once

#include <raylib.h>
#include <entt/entt.hpp>
#include <imgui_entt_entity_editor.hpp>

namespace game::components {

struct BackgroundColor {
    Color color;
};

} // namespace game::components

namespace MM {

template<>
void ComponentEditorWidget<game::components::BackgroundColor>(entt::registry& reg, entt::registry::entity_type e);

} // namespace MM
