#include "./components.hpp"

#include <entt/entity/registry.hpp>

#include "../combat/components.hpp"
#include "../data/components.hpp"
#include "../physics/components.hpp"
#include "../vox/components.hpp"
#include "../vox/resource.hpp"

namespace cfu::comp {

auto create_enemy(entt::registry& reg, entt::entity e) -> void {
    const auto& data = reg.ctx().get<comp::GameData>();
    const auto& desc = data.balance.player.evo_1;
    auto model = reg.ctx().get<res::VoxelModelCache>()[res::vox::Stair];
    reg.emplace<Enemy>(e);
    reg.emplace<comp::Transform>(
        e,
        cfu::comp::Transform {
            .translation = Vector3(0.0f, 10.0f, 0.0f),
            .rotation = Vector3(0.0f, 1.0f, 0.0f),
            .scale = Vector3(4.0f, 4.0f, 4.0f),
        }
    );
    reg.emplace<comp::Position>(e, 0.0f, 0.0f);
    reg.emplace<comp::Velocity>(e, 0.0f, 0.0f);
    reg.emplace<comp::CircleCollider>(e, 8.0f);

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

    reg.emplace<VoxelModel>(e, model);
}

} // namespace cfu::comp
