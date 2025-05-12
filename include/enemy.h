#ifndef ENEMY_H
#define ENEMY_H

#include "raylib.h"
#include "sprites.h"

typedef struct {
    Vector2 position;
    Vector2 velocity;
    int frame;
    float timer;
    int health;
    bool alive;
    float hitTimer;
    EnemySprites sprites; 
} Enemy;

#define DETECTION_RADIUS 200.0f
#define ENEMY_HIT_DURATION 0.2f
#define MIN_DISTANCE_TO_PLAYER 20.0f  // Incluído aqui por clareza

void InitEnemy(Enemy *enemy, Vector2 position);
void UpdateEnemy(Enemy *enemy, Vector2 playerPos, float dt, EnemySprites skeleton, Rectangle playerRect);
Texture2D GetEnemyTexture(Enemy *enemy, EnemySprites skeleton);
Rectangle GetEnemyRect(Enemy *enemy, Texture2D texture);
void DrawEnemy(Enemy *enemy, Texture2D texture);
void DamageEnemy(Enemy *enemy);

#endif
