#pragma once

#include <nlohmann/json_fwd.hpp>
#include <raylib.h>

namespace cfu::comp {

using json = nlohmann::json;

struct PlayerEvo1Data {
    int max_hp = 0;
    int move_speed = 0;
    int damage = 0;
    int attack_speed = 0;
    int attack_radius = 0;
};

void to_json(json& j, const PlayerEvo1Data& d);
void from_json(const json& j, PlayerEvo1Data& d);

struct PlayerEvo2Data {
    int max_hp = 0;
    int move_speed = 0;
    int damage = 0;
    int attack_speed = 0;
    int attack_radius = 0;
};

void to_json(json& j, const PlayerEvo2Data& d);
void from_json(const json& j, PlayerEvo2Data& d);

struct PlayerEvo3Data {
    int max_hp = 0;
    int move_speed = 0;
    int damage = 0;
    int attack_speed = 0;
    int attack_radius = 0;
};

void to_json(json& j, const PlayerEvo3Data& d);
void from_json(const json& j, PlayerEvo3Data& d);

struct PlayerData {
    PlayerEvo1Data evo_1;
    PlayerEvo2Data evo_2;
    PlayerEvo3Data evo_3;
};

void to_json(json& j, const PlayerData& d);
void from_json(const json& j, PlayerData& d);

struct MeleeEnemyData {
    int max_hp = 0;
    int move_speed = 0;
    int damage = 0;
    int attack_speed = 0;
    int attack_radius = 0;
};

void to_json(json& j, const MeleeEnemyData& d);
void from_json(const json& j, MeleeEnemyData& d);

struct EnemiesData {
    MeleeEnemyData melee_1 {};
    MeleeEnemyData melee_2 {};
    MeleeEnemyData melee_3 {};
};

void to_json(json& j, const EnemiesData& d);
void from_json(const json& j, EnemiesData& d);

struct CameraData {
    float box_culling = 40000.0f;
    float lerp = 15.0f;
    Vector3 offset = Vector3(100.0f, 100.0f, 100.0f);
    Vector3 up = Vector3(0.0f, 1.0f, 0.0f);
    float fovy = 200.0f;
    Color ambient = ColorFromHSV(0.0f, 0.0f, 0.1f);
};

void to_json(json& j, const CameraData& d);
void from_json(const json& j, CameraData& d);

struct BalanceData {
    PlayerData player {};
    EnemiesData enemies {};
};

void to_json(json& j, const BalanceData& d);
void from_json(const json& j, BalanceData& d);

struct GameData {
    BalanceData balance {};
    CameraData camera {};
};

} // namespace cfu::comp
