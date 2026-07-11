#pragma once

#include "./resource.hpp"

namespace cfu::comp {

struct VoxelModel {
    res::vox::VoxelModelId id = res::vox::Error;
    entt::resource<res::VoxelModel> handle;

    static auto get(entt::registry& reg, res::vox::VoxelModelId id) -> VoxelModel {
        return VoxelModel {
            .id = id,
            .handle = reg.ctx().get<res::VoxelModelCache>()[id],
        };
    }
};

} // namespace cfu::comp
