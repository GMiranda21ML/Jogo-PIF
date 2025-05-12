#include "enemy.h"
#include <math.h>

void InitEnemy(Enemy *enemy, Vector2 position) {
    enemy->position = position;
    enemy->velocity = (Vector2){0, 0};
    enemy->frame = 0;
    enemy->timer = 0;
    enemy->health = 3;
    enemy->alive = true;
    enemy->hitTimer = 0;
}

void UpdateEnemy(Enemy *enemy, Vector2 playerPos, float dt, EnemySprites skeleton) {
    if (!enemy->alive) return;

    enemy->position.y = 550 - skeleton.walk_right.frames[0].height;

    float xDistance = fabs(playerPos.x - enemy->position.x);
    bool detected = xDistance < DETECTION_RADIUS;

    if (detected) {
        enemy->velocity.x = (enemy->position.x < playerPos.x) ? 50 : -50;
    } else {
        enemy->velocity.x = 0;
    }

    enemy->position.x += enemy->velocity.x * dt;

    enemy->timer += dt;
    if (enemy->timer > skeleton.frame_change) {
        enemy->timer = 0;
        enemy->frame = (enemy->frame + 1) % skeleton.walk_right.frame_count;
    }

    if (enemy->hitTimer > 0) {
        enemy->hitTimer -= dt;
    }
}

Texture2D GetEnemyTexture(Enemy *enemy, EnemySprites skeleton) {
    return (enemy->velocity.x >= 0) ? 
        skeleton.walk_right.frames[enemy->frame] : 
        skeleton.walk_left.frames[enemy->frame];
}

Rectangle GetEnemyRect(Enemy *enemy, Texture2D texture) {
    return (Rectangle){enemy->position.x, enemy->position.y, (float)texture.width, (float)texture.height};
}

void DrawEnemy(Enemy *enemy, Texture2D texture) {
    if (!enemy->alive) return;
    Color tint = (enemy->hitTimer > 0) ? RED : WHITE;
    DrawTexture(texture, (int)enemy->position.x, (int)enemy->position.y, tint);
}

void DamageEnemy(Enemy *enemy) {
    enemy->health--;
    enemy->hitTimer = ENEMY_HIT_DURATION;
    if (enemy->health <= 0) {
        enemy->alive = false;
    }
}
