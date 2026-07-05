#include "./components.hpp"

namespace MM {

template<>
void ComponentEditorWidget<game::components::Position>(entt::registry& reg, entt::registry::entity_type e) {
    auto& position = reg.get<game::components::Position>(e);
    ImGui::DragFloat("X", &position.vec.x);
    ImGui::DragFloat("Y", &position.vec.y);
}

template<>
void ComponentEditorWidget<game::components::Velocity>(entt::registry& reg, entt::registry::entity_type e) {
    auto& velocity = reg.get<game::components::Velocity>(e);
    ImGui::DragFloat("X", &velocity.vec.x);
    ImGui::DragFloat("Y", &velocity.vec.y);
}

} // namespace MM
