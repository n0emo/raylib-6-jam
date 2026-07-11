#include "./components.hpp"

#include <entt/entt.hpp>

#include "../solids/components.hpp"
#include "../physics/components.hpp"
#include "../combat/components.hpp"
#include "../level/components.hpp"
#include "../vox/resource.hpp"
#include "../vox/components.hpp"
#include "../data/components.hpp"

namespace cfu::comp {

using entt::operator""_hs;

auto create_player(entt::registry& reg, entt::entity e) -> void {
    const auto& data = reg.ctx().get<comp::GameData>();
    const auto& desc = data.balance.player.evo_1;

    reg.emplace<Player>(e);
    reg.emplace<comp::Transform>(
        e,
        comp::Transform {
            .translation = Vector3(0.0f, 10.0f, 0.0f),
            .rotation = Vector3(0.0f, 1.0f, 0.0f),
            .scale = Vector3(4.0f, 4.0f, 4.0f),
        }
    );
    reg.emplace<Position>(e, 0.0f, 0.0f);
    reg.emplace<Velocity>(e, 0.0f, 0.0f);
    reg.emplace<CircleCollider>(e, 8.0f);

    reg.emplace<Grounded>(e, 0.0f);

    reg.emplace<Hp>(e, float(desc.max_hp), float(desc.max_hp));
    reg.emplace<MoveSpeed>(e, float(desc.move_speed));
    reg.emplace<MeleeAttack>(
        e,
        MeleeAttack {
            .damage = float(desc.damage),
            .attack_speed = float(desc.attack_speed),
            .attack_radius = float(desc.attack_radius),
        }
    );

    reg.emplace<VoxelModel>(e, VoxelModel::get(reg, res::vox::Player));
    reg.emplace<SolidMaterial>(e, WHITE);
}

} // namespace cfu::comp
