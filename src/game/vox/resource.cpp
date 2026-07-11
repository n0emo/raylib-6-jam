#include "./resource.hpp"

#include <format>

#include "../raymath.hpp"

namespace cfu::res {

auto model_id_to_string(vox::VoxelModelId id) -> gsl::czstring {
    switch (id) {
     #define CFU_X(name, str)                                                                                          \
    case vox::VoxelModelId::name: {                                                                                    \
        return str;                                                                                                    \
    }
        RES_VOX_ID_LIST
     #undef CFU_X
        default:
            return "error.vox";
    }
}

auto model_id_from_string(std::string_view string) -> vox::VoxelModelId {
    #define CFU_X(name, str)                                                                                           \
    if ((str) == string) return vox::VoxelModelId::name;
    RES_VOX_ID_LIST
    #undef CFU_X
    return vox::Error;
}

VoxelModel::VoxelModel(std::string_view path) {
    auto final_path = std::format("assets/vox/{}", path);
    this->model = LoadModel(final_path.c_str());
    const auto bb = GetModelBoundingBox(this->model);
    this->bounding_box = bb;
    const auto x = bb.min.x + (((bb.max.x - bb.min.x) / 2.0f));
    const auto z = bb.min.z + (((bb.max.z - bb.min.z) / 2.0f));
    Matrix matTranslate = MatrixTranslate(-x, 0.0f, -z);
    this->model.transform = matTranslate;
}

VoxelModel::VoxelModel(VoxelModel&& other) noexcept : model(other.model) {
    other.model = {};
}

auto VoxelModel::operator=(VoxelModel&& other) noexcept -> VoxelModel& {
    this->model = other.model;
    other.model = {};
    return *this;
}

VoxelModel::~VoxelModel() {
    if (IsModelValid(this->model)) UnloadModel(this->model);
}

VoxelModel::operator Model() {
    return model;
}

} // namespace cfu::res
