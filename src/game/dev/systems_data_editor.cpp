#include "./systems.hpp"

#include <entt_editor.hpp>
#include <imgui.h>
#include <raylib.h>

#include "../data/components.hpp"
#include "../data/systems.hpp"
#include "../raymath.hpp"
#include "../states.hpp"
#include "./components.hpp"

namespace cfu::systems {

auto setup_data_editor(entt::registry& registry) -> void {
    registry.ctx().insert_or_assign(comp::DataEditor {});
}

template<typename F>
static auto editor_section(gsl::czstring name, F f) {
    if (ImGui::CollapsingHeader(name)) {
        ImGui::PushID(name);
        ImGui::Indent(10.0f);
        f();
        ImGui::Unindent(10.0f);
        ImGui::PopID();
    }
}

template<typename T>
static auto base_stats_editor(T& stats) {
    ImGui::DragInt("Max HP", &stats.max_hp);
    ImGui::DragInt("Move Speed", &stats.move_speed);
    ImGui::DragInt("Damage", &stats.damage);
    ImGui::DragInt("Attack Speed", &stats.attack_speed);
    ImGui::DragInt("Attack Radius", &stats.attack_radius);
}

auto draw_data_editor(entt::registry& registry) -> void {
    auto& d = registry.ctx().get<comp::GameData>();
    auto& editor = registry.ctx().get<comp::DataEditor>();
    if (IsKeyPressed(KEY_F7)) editor.show_window = !editor.show_window;
    if (!editor.show_window) return;

    ImGui::Begin("Data Editor");

    if (ImGui::Button("Apply") || editor.update_realtime) systems::apply_game_data_to_game(registry);
    ImGui::SameLine();
    if (ImGui::Button("Save")) systems::save_game_data_to_disk(registry);
    ImGui::SameLine();
    if (ImGui::Button("Reset")) ImGui::OpenPopup("Confirm Reset?");
    ImGui::SameLine();
    ImGui::Checkbox("Realtime", &editor.update_realtime);

    ImGui::Separator();

    editor_section("Camera settings", [&]() -> void {
        auto& c = d.camera;
        ImGui::DragFloat("Box culling", &c.box_culling, 100.0f);
        ImGui::DragFloat("Lerp factor", &c.lerp, 0.002f);
        ImGui::DragFloat("fovy", &c.fovy, 1.0f);

        ImGui::SeparatorText("Offset");
        ImGui::DragFloat("X##offset", &c.offset.x, 1.0f);
        ImGui::DragFloat("Y##offset", &c.offset.y, 1.0f);
        ImGui::DragFloat("Z##offset", &c.offset.z, 1.0f);

        ImGui::SeparatorText("Up");
        ImGui::DragFloat("X##up", &c.up.x, 1.0f);
        ImGui::DragFloat("Y##up", &c.up.y, 1.0f);
        ImGui::DragFloat("Z##up", &c.up.z, 1.0f);
    });

    editor_section("Player stats", [&]() -> void {
        editor_section("Evo 1", [&]() -> void { base_stats_editor(d.balance.player.evo_1); });
        editor_section("Evo 2", [&]() -> void { base_stats_editor(d.balance.player.evo_2); });
        editor_section("Evo 3", [&]() -> void { base_stats_editor(d.balance.player.evo_3); });
    });

    editor_section("Enemy stats", [&]() -> void {
        editor_section("Melee 1", [&]() -> void { base_stats_editor(d.balance.enemies.melee_1); });
        editor_section("Melee 2", [&]() -> void { base_stats_editor(d.balance.enemies.melee_2); });
        editor_section("Melee 3", [&]() -> void { base_stats_editor(d.balance.enemies.melee_3); });
    });

    if (ImGui::BeginPopupModal("Confirm Reset?", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
        ImGui::Text("Are you sure you want to reset game data?");
        ImGui::Separator();

        if (ImGui::Button("OK", ImVec2(120, 0))) {
            systems::load_game_data_from_disk(registry);
            ImGui::CloseCurrentPopup();
        }

        ImGui::SetItemDefaultFocus();
        ImGui::SameLine();

        if (ImGui::Button("Cancel", ImVec2(120, 0))) {
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }

    ImGui::End();
}

} // namespace cfu::systems
