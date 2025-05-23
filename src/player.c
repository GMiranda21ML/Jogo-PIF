#include "player.h"
#include "map1.h"
#include "maps.h"
#include "levelUp.h"
#include <math.h>
#include "chave.h"
#include "map2.h"
#include <stdlib.h>

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
    player->PossuiKey = false;

    InitPlayerLevels(&player->level);
    player->sprites = LoadPlayerSprites(player->level.currentLevel->spritePath);
    if (player->sprites.frame_change <= 0.0f)
        player->sprites.frame_change = 0.10f;

    player->previousAnim.frames = 0;
}

Rectangle GetPlayerRect(Player *player) {
    Texture2D current;

    if (player->attacking) {
        if (player->attackFacing == 1) {
            current = player->sprites.attack_right.frames[player->frame];
        } else {
            current = player->sprites.attack_left.frames[player->frame];
        }
    } else {
        if (player->facing == 1) {
            current = player->sprites.walk_right.frames[player->frame];
        } else {
            current = player->sprites.walk_left.frames[player->frame];
        }
    }    

    return (Rectangle){player->position.x, player->position.y, (float)current.width, (float)current.height};
}

Rectangle GetPlayerRectWall(Player *player) {
    return (Rectangle){
        player->position.x,
        player->position.y,
        player->sprites.walk_right.frames[0].width,
        player->sprites.walk_right.frames[0].height
    };
}

Rectangle GetPlayerRectFloor(Player *player) {
    return (Rectangle){
        player->position.x,
        player->position.y,
        (float)player->sprites.walk_right.frames[0].width,
        (float)player->sprites.walk_right.frames[0].height
    };
}


void UpdatePlayer(Player *player, float dt, Rectangle *platforms, int platformCount, Rectangle ground, Enemy *enemies, int enemyCount, Sound hitSound, Sound levelUpSound, MapType *currentMap, Sound hitPlayerSound[4]) {

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

    Rectangle playerRectEspinho = GetPlayerRect(player);

    if (player->playerHitTimer > 0.0f) {
        player->playerHitTimer -= dt;
    } else {
        int vidaAntes = player->playerHealth;
        CheckSpikeDamage(playerRectEspinho, &player->playerHealth);

        if (player->playerHealth < vidaAntes) {
            player->playerHitTimer = PLAYER_HIT_COOLDOWN;
        }
    }


    int count = GetWallCount();
    Rectangle *wallsArray = GetWalls();

    Rectangle playerRecWall = GetPlayerRectWall(player);
    for (int i = 0; i < count; i++) {
        if (CheckCollisionRecs(playerRecWall, wallsArray[i])) {
            if (IsKeyDown(KEY_D)) {
                player->position.x -= player->speed * dt;
            } else if (IsKeyDown(KEY_A)) {
                player->position.x += player->speed * dt;
            }
            break;
        }
    }    


    if (player->position.x < 0) player->position.x = 0;

    if (*currentMap == MAP_ORIGINAL && player->position.x + player->sprites.walk_right.frames[0].width >= ground.width) {
        *currentMap = MAP_1;
        player->position = (Vector2){0, 500};
    }
    else if (*currentMap == MAP_1 && player->position.x <= 0) {
        *currentMap = MAP_ORIGINAL;
        player->position = (Vector2){ground.width - player->sprites.walk_right.frames[0].width, 500};
    }
    else if (*currentMap == MAP_1 && player->position.x + player->sprites.walk_right.frames[0].width >= MAP1_WIDTH) {

        player->position.x = MAP1_WIDTH - player->sprites.walk_right.frames[0].width;
    }
    else if (*currentMap == MAP_1 && 
            player->position.y > 550 &&
            ((player->position.x >= 850 && player->position.x <= 1150) ||
            (player->position.x >= 1530 && player->position.x <= 1680))) {

        *currentMap = MAP_2;
        player->position = (Vector2){player->position.x, 0};
    }
    else if (*currentMap == MAP_2 &&
            player->position.y < 0 &&
            ((player->position.x >= 850 && player->position.x <= 1150) ||
            (player->position.x >= 1530 && player->position.x <= 1680))) {

        *currentMap = MAP_1;
        player->position = (Vector2){player->position.x, 520};
    }
    else if (*currentMap == MAP_1 && player->position.x >= 150 && player->position.x <= 250 && player->position.y < 0) {
        *currentMap = MAP_3;
        player->position = (Vector2){player->position.x, 520};
    }
    else if (*currentMap == MAP_3 && player->position.x >= 150 && player->position.x <= 250 && player->position.y > 550) {
        *currentMap = MAP_1;
        player->position = (Vector2){player->position.x, 0};
    }


    player->velocity.y += player->gravity * dt;
    player->position.y += player->velocity.y * dt;
    player->isOnGround = false;
    float playerHeight = (float)player->sprites.walk_right.frames[0].height;
    
    int floorCount   = GetFloorCount();
    Rectangle *floors = GetFloors();
    Rectangle playerRecFloor = GetPlayerRectFloor(player);

    for (int i = 0; i < floorCount; i++) {
        Rectangle ceil = floors[i];

        if (player->velocity.y < 0 && CheckCollisionRecs(playerRecFloor, ceil)) {
            player->position.y = ceil.y + ceil.height;
            player->velocity.y = 0;
            break;
        }
    }
    
    for (int i = 0; i < floorCount; i++) {
        Rectangle ceil = floors[i];

        if (player->velocity.y >= 0 &&
            player->position.y + playerHeight >= ceil.y &&
            player->position.y + playerHeight - player->velocity.y * dt <= ceil.y &&
            player->position.x + player->sprites.walk_right.frames[0].width > ceil.x &&
            player->position.x < ceil.x + ceil.width) {

            player->position.y = ceil.y - playerHeight;
            player->velocity.y = 0;
            player->isOnGround = true;
            break;
        }
    }

    

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

    if (player->position.x + player->sprites.walk_right.frames[0].width > ground.x &&
        player->position.x < ground.x + ground.width &&
        player->position.y + playerHeight >= ground.y) {

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

    UpdateChave(GetMap2Chave(), player);

    Animation currentAnim;
    if (player->attacking) {
        if (player->attackFacing == 1) {
            currentAnim = player->sprites.attack_right;
        } else {
            currentAnim = player->sprites.attack_left;
        }
    } else if (moving) {
        if (player->facing == 1) {
            currentAnim = player->sprites.walk_right;
        } else {
            currentAnim = player->sprites.walk_left;
        }
    } else {
        if (player->facing == 1) {
            currentAnim = player->sprites.idle_right;
        } else {
            currentAnim = player->sprites.idle_left;
        }
    }    

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

            if (player->frame == 1) {
                Rectangle attackRect;
                Texture2D currentAttackFrame;
                if (player->attackFacing == 1) {
                    currentAttackFrame = player->sprites.attack_right.frames[player->frame];
                } else {
                    currentAttackFrame = player->sprites.attack_left.frames[player->frame];
                }                
            
                if (player->attackFacing == 1) {
                    attackRect = (Rectangle){
                        player->position.x + currentAttackFrame.width,
                        player->position.y,
                        30,
                        (float)currentAttackFrame.height
                    };
                } else {
                    attackRect = (Rectangle){
                        player->position.x - 30,
                        player->position.y,
                        30,
                        (float)currentAttackFrame.height
                    };
                }
            
                bool playerNeedsSpriteReload = false;
            
                for (int i = 0; i < enemyCount; i++) {
                    Enemy *enemy = &enemies[i];
                    if (!enemy->alive) continue;
            
                    Texture2D enemyTex = GetEnemyTexture(enemy, enemy->sprites);
                    Rectangle enemyRect = {
                        enemy->position.x,
                        enemy->position.y,
                        (float)enemyTex.width,
                        (float)enemyTex.height
                    };
            
                    if (CheckCollisionRecs(attackRect, enemyRect)) {
                        DamageEnemy(enemy, player->level.currentLevel->damage);
                        PlaySound(hitSound);
            
                        if (!enemy->alive) {
                            AddKill(&player->level, levelUpSound, player);
                            playerNeedsSpriteReload = true;
                        }
                    }
                }
            
                if (playerNeedsSpriteReload) {
                    UnloadPlayerSprites(player->sprites);
                    player->sprites = LoadPlayerSprites(player->level.currentLevel->spritePath);
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

    for (int i = 0; i < enemyCount; i++) {
        Enemy *enemy = &enemies[i];
    
        if (enemy->alive && enemy->attacking && player->playerHitTimer <= 0.0f) {
            Rectangle attackArea = GetEnemyRect(enemy, GetEnemyTexture(enemy, enemy->sprites));
    
            if (enemy->facing >= 0) {
                attackArea.width += 30;
            } else {
                attackArea.x -= 30;
                attackArea.width += 30;
            }
    
            if (CheckCollisionRecs(GetPlayerRect(player), attackArea)) {
                int randomIndex = rand() % 4;
                PlaySound(hitPlayerSound[randomIndex]);
                player->playerHealth -= enemy->damage;
                player->playerHitTimer = PLAYER_HIT_COOLDOWN;
                break; 
            }
        }
    }
    
}

void DrawPlayer(Player *player) {
    Animation currentAnim;
    if (player->attacking) {
        if (player->attackFacing == 1) {
            currentAnim = player->sprites.attack_right;
        } else {
            currentAnim = player->sprites.attack_left;
        }
    } else if (IsKeyDown(KEY_A) || IsKeyDown(KEY_D)) {
        if (player->facing == 1) {
            currentAnim = player->sprites.walk_right;
        } else {
            currentAnim = player->sprites.walk_left;
        }
    } else {
        if (player->facing == 1) {
            currentAnim = player->sprites.idle_right;
        } else {
            currentAnim = player->sprites.idle_left;
        }
    }    

    DrawTexture(currentAnim.frames[player->frame], (int)player->position.x, (int)player->position.y, WHITE);
}

void FreePlayer(Player *player) {
    if (!player) return;
    UnloadPlayerSprites(player->sprites);
}
