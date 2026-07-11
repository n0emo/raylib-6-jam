
#include "./commands.hpp"

namespace cfu::dev {

using namespace std;
using namespace detail;

static auto cmd_state_push(entt::registry& registry, CommandMode mode, Args args) -> std::string;
static auto cmd_state_pop(entt::registry& registry, CommandMode mode, Args args) -> std::string;
static auto cmd_state_replace(entt::registry& registry, CommandMode mode, Args args) -> std::string;

auto cmd_state(entt::registry& registry, CommandMode mode, Args args) -> string {
    if (args.empty() || args[0] == "h" || args[0] == "help") {
        return R"(Usage: state [subcommand] [args]:

Subcommands:
    r, replace <name> - replace current state with `name`
       push    <name> - push `name` to stack
       pop            - pop from stack

Available states:
       main_menu
       in_game
)";
    }
    auto cmd = args[0];
    args = args.size() == 1 ? Args {} : args.subspan(1);

    if (cmd == "r" || cmd == "replace") return cmd_state_replace(registry, mode, args);
    if (cmd == "push") return cmd_state_push(registry, mode, args);
    if (cmd == "pop") return cmd_state_pop(registry, mode, args);

    return format("ERROR: Unknown subcommand `{}`\n", cmd);
}

static auto cmd_state_push(entt::registry& registry, CommandMode mode, Args args) -> string {
    return "ERROR: `push` is not yet implemented\n";
}

static auto cmd_state_pop(entt::registry& registry, CommandMode mode, Args args) -> string {
    return "ERROR: `pop` is not yet implemented\n";
}

static auto cmd_state_replace(entt::registry& registry, CommandMode mode, Args args) -> string {
    return "ERROR: `replace` is not yet implemented\n";
}

} // namespace cfu::dev
