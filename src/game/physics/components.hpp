#pragma once

#include <box2d/box2d.h>
#include <entt/entt.hpp>
#include <imgui_entt_entity_editor.hpp>
#include <raylib.h>

namespace cfu::components {

struct World {
    b2WorldId id;
};

struct Transform {
    Vector3 translation;
    Vector3 rotation;
    Vector3 scale;
};

} // namespace cfu::components

namespace MM {

template<>
void ComponentEditorWidget<cfu::components::Transform>(entt::registry& reg, entt::registry::entity_type e);

} // namespace MM
