#include "./components.hpp"

namespace MM {

template<>
void ComponentEditorWidget<cfu::components::SolidMaterial>(entt::registry& reg, entt::registry::entity_type e) {
    auto& c = reg.get<cfu::components::SolidMaterial>(e);
    auto color = ColorNormalize(c.color);
    ImGui::ColorPicker4("Color", reinterpret_cast<float *>(&color)); // NOLINT
    c.color = ColorFromNormalized(color);
}

template<>
void ComponentEditorWidget<cfu::components::Cube>(entt::registry& reg, entt::registry::entity_type e) {
    auto& c = reg.get<cfu::components::Cube>(e);
    ImGui::DragFloat("Width", &c.size.x);
    ImGui::DragFloat("Height", &c.size.y);
    ImGui::DragFloat("Length", &c.size.z);
}

} // namespace MM
