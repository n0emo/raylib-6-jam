#include "./systems.hpp"

#include <imgui.h>
#include <raylib.h>

#include "../states.hpp"
#include "./components.hpp"
#include "./systems.hpp"
#include "./components.hpp"

namespace cfu::systems {

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
    auto& settings = registry.ctx().get<comp::DevSettings>();
    auto& entt_editor = registry.ctx().get<comp::Editor>();
    auto& tilemap_editor = registry.ctx().get<comp::TileMapEditor>();
    auto& data_editor = registry.ctx().get<comp::DataEditor>();
    auto& console = registry.ctx().get<comp::Console>();
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

            ImGui::Separator();

            ImGui::Text("State Stack");
            auto list = StateStack::get(registry).list();
            for (auto state : std::views::reverse(list)) {
                if (std::holds_alternative<MainMenuState>(state)) ImGui::Text("- Main Menu");
                else if (std::holds_alternative<InGameState>(state)) ImGui::Text("- In Game");
                else ImGui::Text("- Unknown");
            }

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("view")) {
            ImGui::MenuItem("Dev panel [F3]", nullptr, &settings.show_dev_panel);
            ImGui::MenuItem("Entity editor [F5]", nullptr, &entt_editor.show_window);
            ImGui::MenuItem("Tilamap editor [F6]", nullptr, &tilemap_editor.show_window);
            ImGui::MenuItem("Data editor [F7]", nullptr, &data_editor.show_window);
            ImGui::MenuItem("Console [`]", nullptr, &data_editor.show_window);
            ImGui::EndMenu();
        }

        ImGui::PopItemFlag();
        ImGui::EndMenuBar();
    }
    ImGui::End();
}

} // namespace cfu::systems
