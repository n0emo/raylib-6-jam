#include "./systems.hpp"

#include <imgui_entt_entity_editor.hpp>
#include <imgui.h>
#include <raylib.h>

#include "../background/components.hpp"
#include "../physics/components.hpp"
#include "../player/components.hpp"
#include "../shapes/components.hpp"
#include "../states.hpp"
#include "./components.hpp"

namespace game::systems {

auto setup_dev(entt::registry& registry) -> void {
    registry.ctx().insert_or_assign(components::DevSettings {});
    setup_entt_editor(registry);
}

auto draw_dev(entt::registry& registry) -> void {
    auto& settings = registry.ctx().get<components::DevSettings>();
    if (!settings.show_dev_panel) return;
    draw_dev_panel(registry);
    draw_entt_editor(registry);
}

auto update_dev(entt::registry& registry) -> void {
    auto& settings = registry.ctx().get<components::DevSettings>();
    if (IsKeyPressed(KEY_F3)) settings.show_dev_panel = !settings.show_dev_panel;
}

struct StateMenuResult {
    bool main_menu = false;
    bool in_game = false;
};

static auto state_menu() -> StateMenuResult {
    auto result = StateMenuResult {};

    if (ImGui::MenuItem("Main menu")) result.main_menu = true;
    if (ImGui::MenuItem("In game")) result.in_game = true;

    return result;
}

auto draw_dev_panel(entt::registry& registry) -> void {
    // auto& settings = registry.ctx().get<components::DevSettings>();
    auto& editor = registry.ctx().get<components::Editor>();
    auto& stack = StateStack::get(registry);

    auto io = ImGui::GetIO();
    auto flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoTitleBar
        | ImGuiWindowFlags_NoResize;
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(io.DisplaySize);
    ImGui::Begin("Dev panel", nullptr, flags);
    if (ImGui::BeginMenuBar()) {
        ImGui::PushItemFlag(ImGuiItemFlags_AutoClosePopups, false);

        if (ImGui::BeginMenu("state")) {
            if (ImGui::BeginMenu("Push")) {
                auto result = state_menu();
                if (result.main_menu) stack.push(MainMenuState {});
                if (result.in_game) stack.push(InGameState {});
                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Replace")) {
                auto result = state_menu();
                if (result.main_menu) stack.replace(MainMenuState {});
                if (result.in_game) stack.replace(InGameState {});
                ImGui::EndMenu();
            }

            if (ImGui::MenuItem("Pop")) stack.pop();

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("view")) {
            ImGui::MenuItem("Entity editor", nullptr, &editor.show_window);
            ImGui::EndMenu();
        }

        ImGui::PopItemFlag();
        ImGui::EndMenuBar();
    }
    ImGui::End();
}

auto setup_entt_editor(entt::registry& registry) -> void {
    auto editor = components::Editor {};
    editor.show_window = false;
    editor.registerComponent<components::BackgroundColor>("BackgroundColor");
    editor.registerComponent<components::Position>("Position");
    editor.registerComponent<components::Velocity>("Velocity");
    editor.registerComponent<components::ShapeColor>("Shape Color");
    editor.registerComponent<components::Ball>("Ball");
    editor.registerComponent<components::Player>("Player");
    registry.ctx().insert_or_assign(editor);

    auto entity = components::CurrentEntity {.entity = entt::null};
    registry.ctx().insert_or_assign(entity);
}

auto draw_entt_editor(entt::registry& registry) -> void {
    auto& entity = registry.ctx().get<components::CurrentEntity&>().entity;
    auto& editor = registry.ctx().get<components::Editor>();
    editor.renderSimpleCombo(registry, entity);
}

} // namespace game::systems
