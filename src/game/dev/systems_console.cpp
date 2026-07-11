#include "./systems.hpp"

#include <cstring>

#include <imgui.h>

#include "../solids/components.hpp"
#include "./commands.hpp"
#include "./components.hpp"

namespace cfu::systems {

using namespace std;

static auto input_callback(ImGuiInputTextCallbackData *data) -> int;

auto setup_console(entt::registry& registry) -> void {
    registry.ctx().insert_or_assign(comp::Console {});
    auto cursor = registry.create();
    registry.emplace<comp::CursorCube>(cursor);
    registry.emplace<comp::Transform>(
        cursor,
        comp::Transform {
            .translation = {},
            .rotation = {},
            .scale = Vector3One() * 1.02f,
        }
    );
    registry.emplace<comp::Cube>(cursor, comp::Cube {});
    registry.emplace<comp::SolidMaterial>(cursor, comp::SolidMaterial {});
}

auto draw_console(entt::registry& registry) -> void {
    auto& console = registry.ctx().get<comp::Console>();
    if (IsKeyPressed(KEY_GRAVE)) console.show_window = !console.show_window;
    if (!console.show_window) return;

    ImGui::Begin("Console", &console.show_window);

    const float footer_height = 32.0f;

    ImGui::BeginChild("Output", ImVec2(0, -footer_height), ImGuiChildFlags_Borders);
    ImGui::TextUnformatted(console.output.c_str());
    if (console.scroll_output) {
        ImGui::SetScrollHereY(1.0f);
        console.scroll_output = false;
    }
    ImGui::EndChild();

    ImGui::BeginChild("Input");
    ImGui::Separator();

    ImGui::Text(">");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(-1.0f);
    auto entered = ImGui::InputText(
        "##command",
        console.command_buf.data(),
        console.command_buf.size(),
        ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CallbackHistory,
        input_callback,
        static_cast<void *>(&console)
    );

    auto command = string(console.command_buf.data());
    if (!command.empty()) {
        dev::handle_command(registry, dev::CommandMode::Preview, command);
    }

    if (entered && strlen(console.command_buf.data()) > 0) {
        ImGui::SetKeyboardFocusHere(-1);

        console.output += "> ";
        console.output += command;
        console.output += '\n';
        console.output += dev::handle_command(registry, dev::CommandMode::Commit, command);
        console.output += '\n';
        if (console.output[0] == '\n') console.output.clear();
        console.command_history.emplace_back(std::move(command));
        console.command_buf.fill('\0');

        console.history_index = gsl::index(console.command_history.size());
        console.scroll_output = true;
    }

    ImGui::EndChild();

    ImGui::End();
}

static auto input_callback(ImGuiInputTextCallbackData *data) -> int {
    auto& console = *static_cast<comp::Console *>(data->UserData);
    switch (data->EventFlag) {
        case ImGuiInputTextFlags_CallbackHistory: {
            auto update_input = false;
            if (data->EventKey == ImGuiKey_UpArrow && console.history_index > 0) {
                console.history_index -= 1;
                update_input = true;
            } else if (
                data->EventKey == ImGuiKey_DownArrow
                && console.history_index < gsl::index(console.command_history.size()) - 1
            ) {
                console.history_index += 1;
                update_input = true;
            } else if (console.history_index == gsl::index(console.command_history.size()) - 1) {
                console.history_index += 1;
                memset(data->Buf, '\0', data->BufSize);
                data->BufTextLen = 0;
                data->BufDirty = true;
            }

            if (update_input) {
                data->BufTextLen = int(console.command_history[console.history_index].size());
                data->CursorPos = data->BufTextLen;
                memset(data->Buf, '\0', data->BufSize);
                memcpy(
                    data->Buf,
                    console.command_history[console.history_index].data(),
                    console.command_history[console.history_index].size()
                );
                data->BufDirty = true;
            }
            break;
        }
        default:
            break;
    }

    return 0;
}

} // namespace cfu::systems
