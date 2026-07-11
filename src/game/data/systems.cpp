#include "./systems.hpp"

#include <fstream>

#include <entt/entity/registry.hpp>
#include <nlohmann/json.hpp>

#include "../constants.hpp"
#include "../camera/components.hpp"
#include "../combat/components.hpp"
#include "../player/components.hpp"
#include "./components.hpp"

namespace cfu::systems {

auto save_game_data_to_disk(entt::registry& registry) -> void {
    auto d = registry.ctx().get<comp::GameData>();

    {
        std::ofstream file {BALANCE_DATA_PATH};
        comp::json j = d.balance;
        file << std::setw(2) << j << std::endl;
    }

    {
        std::ofstream file {CAMERA_DATA_PATH};
        comp::json j = d.camera;
        file << std::setw(2) << j << std::endl;
    }
}

auto load_game_data_from_disk(entt::registry& registry) -> void {
    auto d = comp::GameData {};

    {
        std::ifstream file {BALANCE_DATA_PATH};
        if (file.is_open()) {
            comp::json j {};
            file >> j;
            d.balance = j;
        } else {
            TraceLog(LOG_ERROR, "Balance data %s was not found", BALANCE_DATA_PATH);
        }
    }

    {
        std::ifstream file {CAMERA_DATA_PATH};
        if (file.is_open()) {
            comp::json j {};
            file >> j;
            d.camera = j;
        } else {
            TraceLog(LOG_ERROR, "Balance data %s was not found", CAMERA_DATA_PATH);
        }
    }

    registry.ctx().insert_or_assign(d);

    apply_game_data_to_game(registry);
}

auto apply_game_data_to_game(entt::registry& registry) -> void {
    apply_game_camera_data_to_game(registry);
    apply_game_player_data_to_game(registry);
}

auto apply_game_camera_data_to_game(entt::registry& registry) -> void {
    const auto& d = registry.ctx().get<comp::GameData>();
    auto view = registry.view<Camera3D, comp::CameraLerp, comp::CameraOffset>();
    for (auto [entity, camera, lerp, offset] : view.each()) {
        camera.fovy = d.camera.fovy;
        camera.up = d.camera.up;
        lerp.amount = d.camera.lerp;
        offset.offset = d.camera.offset;
    }
}

auto apply_game_player_data_to_game(entt::registry& registry) -> void {
    const auto& d = registry.ctx().get<comp::GameData>();
    // TODO: Handle player evo 2 and evo 3
    const auto& p = d.balance.player.evo_1;
    auto view = registry.view<comp::Player, comp::Hp, comp::MoveSpeed, comp::MeleeAttack>();
    for (auto [entity, hp, ms, melee] : view.each()) {
        if (int(hp.max) != p.max_hp) {
            hp.max = float(p.max_hp);
            hp.current = float(p.max_hp);
        }
        ms.value = float(p.move_speed);
        melee.damage = float(p.damage);
        melee.attack_speed = float(p.attack_speed);
        melee.attack_radius = float(p.attack_radius);
    }
}

} // namespace cfu::systems
