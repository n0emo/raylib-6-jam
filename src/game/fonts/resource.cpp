#include "./resource.hpp"

#include <format>
#include "raylib.h"

namespace cfu::res {

// TODO: implement font resources

CfuFont::CfuFont(std::string_view path) {
    auto final_path = std::format("assets/fonts/{}", path);
    this-> font = LoadFont(final_path.c_str());
}

CfuFont::CfuFont(CfuFont&& other) noexcept : font(other.font) {
    other.font= {};
}

auto CfuFont::operator=(CfuFont&& other) noexcept -> CfuFont& {
    this->font= other.font;
    other.font= {};
    return *this;
}

CfuFont::~CfuFont() {
    if (IsFontValid(this->font)) UnloadFont(this->font);
}

CfuFont::operator Font() {
    return font;
}
} // namespace cfu::res
