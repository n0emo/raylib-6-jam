#include "./commands.hpp"

#include "../level/components.hpp"
#include "../level/systems.hpp"
#include "../shaders/components.hpp"
#include "../vox/resource.hpp"

namespace cfu::dev {

using namespace std;
using namespace detail;

static auto cmd_level_tiles(entt::registry& registry, CommandMode mode, Args args) -> string;
static auto cmd_level_props(entt::registry& registry, CommandMode mode, Args args) -> string;
static auto cmd_level_lights(entt::registry& registry, CommandMode mode, Args args) -> string;
static auto cmd_level_spawners(entt::registry& registry, CommandMode mode, Args args) -> string;
static auto cmd_level_save(entt::registry& registry, CommandMode mode, Args args) -> string;

auto cmd_level(entt::registry& registry, CommandMode mode, Args args) -> std::string {
    if (args.empty() || args[0] == "h" || args[0] == "help") {
        return R"(Usage: level [subcommand] [args]:

Subcommands:
    t, tiles    - manage level tiles
    p, props    - manage level props
    l, lights   - manage level lights
    s, spawners - manage level spawners
       save     - save current level to file
)";
    }
    auto cmd = args[0];
    args = args.size() == 1 ? Args {} : args.subspan(1);

    if (cmd == "t" || cmd == "tiles") return cmd_level_tiles(registry, mode, args);
    if (cmd == "p" || cmd == "props") return cmd_level_props(registry, mode, args);
    if (cmd == "l" || cmd == "lights") return cmd_level_lights(registry, mode, args);
    if (cmd == "s" || cmd == "spawners") return cmd_level_spawners(registry, mode, args);
    if (cmd == "save") return cmd_level_save(registry, mode, args);

    return format("ERROR: Unknown subcommand `{}`\n", cmd);
}

static auto cmd_level_tiles(entt::registry& reg, CommandMode mode, Args args) -> string {
    (void)reg;
    if (args.empty() || args[0] == "h" || args[0] == "help") {
        return R"(Usage: level tiles [subcommand] [args]:

Subcommands:
    c, create <kind> <model> <x y z>             - create tile at (x, y, z)
    c, create <kind> <model> <x1 y1 z1 x2 y2 z2> - create tiles from (x1, y2, z2) to (x2, y2, z2)
    d, delete <x y z>             - delete tile at (x, y, z)
    d, delete <x1 y1 z1 x2 y2 z2> - delete tiles from (x1, y2, z2) to (x2, y2, z2)
    r, rotate <x y z>             - rotate tile at (x, y, z)
    r, rotate <x1 y1 z1 x2 y2 z2> - rotate tiles from (x1, y2, z2) to (x2, y2, z2)
)";
    }

    auto cmd = args[0];
    args = args.size() == 1 ? Args {} : args.subspan(1);
    if (cmd == "c" || cmd == "create") {
        if (args.size() != 5 && args.size() != 8) {
            return format("ERROR: Incorrect number of args\nNeed 5 or 8, got {}\n", args.size());
        }

        auto kind = res::tile_kind_from_string(args[0]);
        auto model = res::model_id_from_string(format("{}.vox", args[1]));
        args = args.subspan(2);
        return tile_or_region_action(reg, mode, args, [&](auto x, auto y, auto z) -> string {
            auto e = reg.create();
            comp::create_tile(reg, e, TileIndex(x, y, z), kind, res::TileRotation::None, model);
            // TODO: update heightmap
            return format("Created tile ({}, {}, {})\n", x, y, z);
        });
    } else if (cmd == "d" || cmd == "delete") {
        return tile_or_region_action(reg, mode, args, [](auto x, auto y, auto z) -> string {
            return format("Deleted tile ({}, {}, {})\n", x, y, z);
        });
    } else if (cmd == "r" || cmd == "rotate") {
        return tile_or_region_action(reg, mode, args, [](auto x, auto y, auto z) -> string {
            return format("Rotated tile ({}, {}, {})\n", x, y, z);
        });
    }

    return format("ERROR: Unknown subcommand `{}`\n", cmd);
}

static auto cmd_level_props(entt::registry& reg, CommandMode mode, Args args) -> string {
    (void)reg;
    if (args.empty() || args[0] == "h" || args[0] == "help") {
        return R"(Usage: level props [subcommand] [args]:

Subcommands:
    <l|list>           - list all props
    <c|create> <x y z> - create new prop at tile (x, y, z)
)";
    }

    auto cmd = args[0];
    args = args.size() == 1 ? Args {} : args.subspan(1);
    if (cmd == "l" || cmd == "list") {
        return format("Props list\n");
    } else if (cmd == "c" || cmd == "create") {
        if (args.size() != 4) {
            return format("ERROR: Incorrect number of args\nNeed 4, got {}\n", args.size());
        }

        const auto model_name = format("{}.vox", args[0]);
        args = args.size() == 1 ? Args {} : args.subspan(1);
        return tile_action(reg, mode, args, [&](auto x, auto y, auto z) -> string {
            auto prop = reg.create();
            const auto model_id = res::model_id_from_string(model_name);
            const auto position = Vector3(float(x), float(y), float(z)) * TILE_SIZE;
            comp::create_prop(reg, prop, position, model_id);
            auto& current_entity = reg.ctx().get<comp::CurrentEntity>();
            current_entity.entity = prop;
            return format("Created prop at ({}, {}, {})", x, y, z);
        });
    }

    return format("ERROR: Unknown subcommand `{}`\n", cmd);
}

static auto cmd_level_lights(entt::registry& reg, CommandMode mode, Args args) -> string {
    if (args.empty() || args[0] == "h" || args[0] == "help") {
        return R"(Usage: level lights [subcommand] [args]:

Subcommands:
    <l|list>           - list all lights
    <c|create> <x y z> - create new light at tile (x, y, z)
)";
    }

    auto cmd = args[0];
    args = args.size() == 1 ? Args {} : args.subspan(1);
    if (cmd == "l" || cmd == "list") {
        return format("Lights list\n");
    } else if (cmd == "c" || cmd == "create") {
        return tile_action(reg, mode, args, [&](auto x, auto y, auto z) -> string {
            auto light = reg.create();
            const auto position = Vector3(float(x), float(y), float(z)) * TILE_SIZE;
            comp::create_level_light(reg, light, position, GRAY);
            auto& current_entity = reg.ctx().get<comp::CurrentEntity>();
            current_entity.entity = light;
            comp::set_lights_shader_locations(reg, comp::get_lighting_shader(reg));
            return format("Created light at tile ({}, {}, {})\n", x, y, z);
        });
    }

    return format("ERROR: Unknown subcommand `{}`\n", cmd);
    (void)reg;
}

static auto cmd_level_spawners(entt::registry& registry, CommandMode mode, Args args) -> string {
    if (mode != CommandMode::Commit) return "";
    (void)registry;
    (void)args;
    return "ERROR: Command `spawners` not yet implemented\n";
}

static auto cmd_level_save(entt::registry& reg, CommandMode mode, Args args) -> string {
    (void)args;
    if (mode != CommandMode::Commit) return "";
    auto id_e = reg.view<comp::CurrentLevel>().back();
    if (id_e == entt::null) return "ERROR: Current level component was not in registry";
    auto id = reg.get<comp::CurrentLevel>(id_e).id;
    auto level = comp::serialize_level(reg);
    reg.ctx().get<res::LevelCache>()[id]->level = level;
    systems::save_levels(reg);
    return "Level successfully saved";
}

} // namespace cfu::dev
