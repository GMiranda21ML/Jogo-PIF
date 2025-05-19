#include "player.h"
#include "map1.h"
#include "maps.h"
#include <math.h>

#define PLAYER_HIT_COOLDOWN 0.5f

void InitPlayer(Player *player) {
    player->position = (Vector2){400, 500};
    player->velocity = (Vector2){0, 0};
    player->gravity = 900.0f;
    player->jumpForce = -450.0f;
    player->speed = 200.0f;
    player->timer = 0;
    player->frame = 0;
    player->facing = 1;
    player->attacking = false;
    player->attackFacing = 1;
    player->isOnGround = false;
    player->playerHitTimer = 0.0f;
    player->playerHealth = 150;

    InitPlayerLevels(&player->level);
    player->sprites = LoadPlayerSprites(player->level.currentLevel->spritePath);
    if (player->sprites.frame_change <= 0.0f)
        player->sprites.frame_change = 0.10f;

    player->previousAnim.frames = 0;
}

Rectangle GetPlayerRect(Player *player) {
    Texture2D current = player->attacking ?
        ((player->attackFacing == 1) ? player->sprites.attack_right.frames[player->frame] : player->sprites.attack_left.frames[player->frame]) :
        ((player->facing == 1) ? player->sprites.walk_right.frames[player->frame] : player->sprites.walk_left.frames[player->frame]);

    return (Rectangle){player->position.x, player->position.y, (float)current.width, (float)current.height};
}

void UpdatePlayer(Player *player, float dt, Rectangle *platforms, int platformCount, Rectangle ground, Enemy *enemy, Sound hitSound, Sound levelUpSound, MapType *currentMap) {
    bool moving = false;
    if (player->playerHitTimer > 0.0f) player->playerHitTimer -= dt;

    if (IsKeyDown(KEY_D)) {
        player->position.x += player->speed * dt;
        player->facing = 1;
        moving = true;
    } else if (IsKeyDown(KEY_A)) {
        player->position.x -= player->speed * dt;
        player->facing = -1;
        moving = true;
    }

    if (player->position.x < 0) player->position.x = 0;

    if (*currentMap == MAP_ORIGINAL && player->position.x + player->sprites.walk_right.frames[0].width >= ground.width) {
        *currentMap = MAP_1;
        ground = ground1;
        for (int i = 0; i < MAP1_PLATFORM_COUNT; i++) {
            platforms[i] = platforms1[i];
        }
        player->position = (Vector2){0, 500};
    } else if (*currentMap == MAP_1 && player->position.x <= 0) {
        *currentMap = MAP_ORIGINAL;
        ground = (Rectangle){0, 550, 2000, 500};
        for (int i = 0; i < platformCount; i++) {
            platforms[i] = (Rectangle){ 200 + i*250, 450 - i*100, 150, 20 };
        }
        player->position = (Vector2){ground.width - player->sprites.walk_right.frames[0].width, 500};
        InitEnemy(enemy, (Vector2){600, 500});
        enemy->sprites = LoadEnemySprites("assets/sprites/enemy/skeleton_green/skeleton_green.json");
    }

    player->velocity.y += player->gravity * dt;
    player->position.y += player->velocity.y * dt;
    player->isOnGround = false;
    float playerHeight = (float)player->sprites.walk_right.frames[0].height;

    for (int i = 0; i < platformCount; i++) {
        Rectangle plat = platforms[i];
        if (player->velocity.y >= 0 &&
            player->position.y + playerHeight >= plat.y &&
            player->position.y + playerHeight - player->velocity.y * dt <= plat.y &&
            player->position.x + player->sprites.walk_right.frames[0].width > plat.x &&
            player->position.x < plat.x + plat.width) {

            player->position.y = plat.y - playerHeight;
            player->velocity.y = 0;
            player->isOnGround = true;
        }
    }

    if (player->position.y + playerHeight >= ground.y) {
        player->position.y = ground.y - playerHeight;
        player->velocity.y = 0;
        player->isOnGround = true;
    }

    if (player->isOnGround && IsKeyPressed(KEY_W)) {
        player->velocity.y = player->jumpForce;
        player->isOnGround = false;
    }

    if (IsKeyPressed(KEY_L) && !player->attacking) {
        player->attacking = true;
        player->frame = 0;
        player->timer = 0;
        player->attackFacing = player->facing;
    }

    Animation currentAnim;
    if (player->attacking)
        currentAnim = (player->attackFacing == 1) ? player->sprites.attack_right : player->sprites.attack_left;
    else if (moving)
        currentAnim = (player->facing == 1) ? player->sprites.walk_right : player->sprites.walk_left;
    else
        currentAnim = (player->facing == 1) ? player->sprites.idle_right : player->sprites.idle_left;

    if (currentAnim.frames != player->previousAnim.frames) {
        player->frame = 0;
        player->timer = 0;
        player->previousAnim = currentAnim;
    }

    player->timer += dt;
    while (player->timer > player->sprites.frame_change) {
        player->timer -= player->sprites.frame_change;
        if (player->attacking) {
            player->frame++;

            if (player->frame == 1 && enemy->alive) {
                Rectangle playerRect = GetPlayerRect(player);
                Texture2D skeletonTex = GetEnemyTexture(enemy, enemy->sprites);
                Rectangle skeletonRect = {enemy->position.x, enemy->position.y, (float)skeletonTex.width, (float)skeletonTex.height};

                if (CheckCollisionRecs(playerRect, skeletonRect) &&
                    player->attackFacing == ((enemy->position.x > player->position.x) ? 1 : -1)) {

                    DamageEnemy(enemy, player->level.currentLevel->damage);
                    PlaySound(hitSound);

                    if (!enemy->alive) {
                        AddKill(&player->level, levelUpSound);
                        UnloadPlayerSprites(player->sprites);
                        player->sprites = LoadPlayerSprites(player->level.currentLevel->spritePath);
                    }
                }
            }

            if (player->frame >= currentAnim.frame_count) {
                player->frame = 0;
                player->attacking = false;
            }
        } else {
            player->frame = (player->frame + 1) % currentAnim.frame_count;
        }
    }

    if (enemy->alive && enemy->attacking && player->playerHitTimer <= 0.0f) {
        Rectangle attackArea = GetEnemyRect(enemy, GetEnemyTexture(enemy, enemy->sprites));
        if (enemy->facing >= 0) attackArea.width += 30;
        else {
            attackArea.x -= 30;
            attackArea.width += 30;
        }

        if (CheckCollisionRecs(GetPlayerRect(player), attackArea)) {
            player->playerHealth -= 10;
            player->playerHitTimer = PLAYER_HIT_COOLDOWN;
        }
    }
}

void DrawPlayer(Player *player) {
    Animation currentAnim;
    if (player->attacking)
        currentAnim = (player->attackFacing == 1) ? player->sprites.attack_right : player->sprites.attack_left;
    else if (IsKeyDown(KEY_A) || IsKeyDown(KEY_D))
        currentAnim = (player->facing == 1) ? player->sprites.walk_right : player->sprites.walk_left;
    else
        currentAnim = (player->facing == 1) ? player->sprites.idle_right : player->sprites.idle_left;

    DrawTexture(currentAnim.frames[player->frame], (int)player->position.x, (int)player->position.y, WHITE);
}

void FreePlayer(Player *player) {
    if (!player) return;
    UnloadPlayerSprites(player->sprites);
}
