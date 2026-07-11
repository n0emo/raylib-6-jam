#include "./commands.hpp"

#include "../solids/components.hpp"
#include "./components.hpp"

namespace cfu::dev {

using namespace std;

auto handle_command(entt::registry& registry, CommandMode mode, string_view command) -> string try {
    auto cursor = registry.view<comp::CursorCube>().back();
    auto& cube = registry.get<comp::Cube>(cursor);
    cube.size = Vector3Zero();

    if (mode == CommandMode::Commit) {
        TraceLog(LOG_INFO, "CMD: %.*s", int(command.size()), command.data());
    }
    auto& console = registry.ctx().get<comp::Console>();

    vector<string_view> args_vec {};
    auto parts = command | views::split(' ');
    for (const auto& part : parts) {
        if (part.empty()) continue;
        args_vec.emplace_back(part);
    }
    if (args_vec.empty()) return "ERROR: empty command";

    auto args = span(args_vec);
    auto cmd = args[0];
    args = args.size() == 1 ? span<string_view> {} : args.subspan(1);

    // Builtins
    if (cmd == "h" || cmd == "help") {
        return R"(Builtins:
    h, help  - print this page
       clear - clear console output

Commands:
    l, level - edit level
    s, state - manage game state
)";
    }

    if (cmd == "clear") {
        console.output.clear();
        return "";
    }

    // Commands
    if (cmd == "l" || cmd == "level") return cmd_level(registry, mode, args);
    if (cmd == "s" || cmd == "state") return cmd_state(registry, mode, args);
    return format("ERROR: Unknown command `{}`.\nType `help` to get the list of avaliable commands\n", cmd);
} catch (const std::exception& e) {
    return format("ERROR: {}", e.what());
}

} // namespace cfu::dev
