#ifndef SPRITES_H
#define SPRITES_H

#include "raylib.h"

typedef struct {
    Texture2D *frames;
    int frame_count;
} Animation;

typedef struct {
    Animation walk_right;
    Animation walk_left;
    Animation idle_right;
    Animation idle_left;
    Animation attack_right;
    Animation attack_left;
    float frame_change;
} PlayerSprites;

typedef struct {
    Texture2D *frames;
    int frame_count;
} GroundGrassSprites;

typedef struct {
    Animation walk_right;
    Animation walk_left;
    Animation attack_right;
    Animation attack_left;
    float frame_change;
} EnemySprites;

typedef struct {
    Animation walk_right;
    Animation walk_left;
    Animation attack_right1;
    Animation attack_left1;
    Animation attack_right2;
    Animation attack_left2;
    Animation attack_right3;
    Animation attack_left3;
    Animation attack_right4;
    Animation attack_left4;
    float frame_change;
} BossSprites;

PlayerSprites LoadPlayerSprites(const char *jsonPath);
void UnloadPlayerSprites(PlayerSprites sprites);

GroundGrassSprites LoadGroundSprites(const char *jsonPath);
void UnloadGroundSprites(GroundGrassSprites ground);

EnemySprites LoadEnemysSprites(const char *jsonPath);
void UnloadEnemysSprites(EnemySprites sprites);

EnemySprites LoadEnemySprites(const char *jsonPath);
void UnloadEnemySprites(EnemySprites sprites);

BossSprites BossLoadSprites(const char *jsonPath);
void BossUnloadSprites(BossSprites s);
BossSprites LoadBossSprites(const char *jsonPath);
void UnloadBossSprites(BossSprites s);

Texture2D FlipTextureHorizontally(Texture2D original);

#endif
