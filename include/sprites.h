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



PlayerSprites LoadPlayerSprites(const char *jsonPath);
void UnloadPlayerSprites(PlayerSprites sprites);
Texture2D FlipTextureHorizontally(Texture2D original);
GroundGrassSprites LoadGroundSprites(const char *jsonPath);
void UnloadGroundSprites(GroundGrassSprites ground);

#endif
