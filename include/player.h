#ifndef PLAYER_H
#define PLAYER_H

#include "raylib.h"
#include "sprites.h"
#include "levelUp.h"
#include "enemy.h"
#include "maps.h"
#include "chave.h"

#define PLAYER_HIT_COOLDOWN 0.5f
#define MAP1_WIDTH 2000 

typedef struct Player {
    Vector2 position;
    Vector2 velocity;
    float gravity;
    float jumpForce;
    float speed;
    float timer;
    int frame;
    int facing;
    int attackFacing;
    bool attacking;
    bool isOnGround;
    float playerHitTimer;
    int playerHealth;
    PlayerSprites sprites;
    Animation previousAnim;
    PlayerLevel level;
    bool PossuiKey;
} Player;

void InitPlayer(Player *player);
void UpdatePlayer(Player *player, float dt, Rectangle *platforms, int platformCount, Rectangle ground, Enemy *enemies, int enemyCount, Sound hitSound, Sound levelUpSound, MapType *currentMap, Sound hitPlayerSound[6]);
void DrawPlayer(Player *player);
void FreePlayer(Player *player);
Rectangle GetPlayerRect(Player *player);

#endif
