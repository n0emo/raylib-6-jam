#pragma once

#include <entt/entity/registry.hpp>
#include <entt_editor.hpp>

#include <level/resource.hpp>

namespace cfu::comp {

struct DevSettings {
    bool show_dev_panel = true;
};

using Editor = EntityEditor<entt::entity>;

struct CurrentEntity {
    entt::entity entity;
};

enum class TileMapEditorMode {
    Free,
    Selecting,
    Placing,
    Erasing,
};

struct TileMapEditor {
    bool show_window = false;
    bool enabled = false;
    TileMapEditorMode mode = TileMapEditorMode::Free;
    res::TileKind current_tile = res::TileKind::Wall;
};

struct DataEditor {
    bool show_window = false;
    bool update_realtime = true;
};

struct Console {
    bool show_window = false;
    std::array<char, 128> command_buf {};
    std::string output {};
    bool scroll_output = false;
    gsl::index history_index {};
    std::vector<std::string> command_history {};
};

struct CursorCube {};

} // namespace cfu::comp
