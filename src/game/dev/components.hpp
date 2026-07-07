#pragma once

#include <entt/entt.hpp>
#include <imgui_entt_entity_editor.hpp>

namespace cfu::components {

struct DevSettings {
    bool show_dev_panel = true;
};

using Editor = MM::EntityEditor<entt::entity>;

struct CurrentEntity {
    entt::entity entity;
};

} // namespace cfu::components
