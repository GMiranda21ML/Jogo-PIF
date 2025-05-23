#include "enemy.h"
#include "maps.h"
#include <math.h>

void InitEnemy(Enemy *enemy, Vector2 position, float attackVelocity, int health, int damage, int walkVelocity) {
    enemy->position = position;
    enemy->velocity = (Vector2){0, 0};
    enemy->frame = 0;
    enemy->timer = 0;
    enemy->health = health;
    enemy->alive = true;
    enemy->hitTimer = 0;
    enemy->attacking = false;
    enemy->facing = 1;
    enemy->attackVelocity = attackVelocity;
    enemy->damage = damage;
    enemy->walkVelocity = walkVelocity;
}

void UpdateEnemy(Enemy *enemy, Vector2 playerPos, float dt, EnemySprites enemySprites, Rectangle playerRect) {
    if (!enemy->alive) return;

    float deltaX = enemy->position.x - playerPos.x;
    float deltaY = enemy->position.y - playerPos.y;
    float xDistance = sqrt(deltaX * deltaX + deltaY * deltaY);
    bool detected = xDistance < DETECTION_RADIUS;
    bool tooClose = xDistance < MIN_DISTANCE_TO_PLAYER;
    bool alignedVertically = fabsf(enemy->position.y - playerPos.y) < LINE_ALIGNMENT_TOLERANCE;

    if (detected && alignedVertically) {
        float sign = playerPos.x - enemy->position.x < 0 ? -1 : 1;
        enemy->velocity.x = sign * enemy->walkVelocity;
        enemy->facing = sign;
        enemy->attacking = tooClose;
    } else {
        enemy->velocity.x = 0;
        enemy->attacking = false;
    }

    if (fabsf(enemy->position.x - playerPos.x) > DETECTION_RADIUS) {
        return;
    }

    if (tooClose)
        enemy->velocity.x = 0;
    // Verifica colisão com as paredes
    bool colidiuComParede = false;
    Rectangle *walls = GetWalls();
    int wallCount = GetWallCount();

    Vector2 proposedPosition = enemy->position;
    proposedPosition.x += enemy->velocity.x * dt;

    Texture2D currentTex = GetEnemyTexture(enemy, enemySprites);

    Rectangle proposedRectWall = {
        proposedPosition.x,
        enemy->position.y,
        (float)currentTex.width,
        (float)currentTex.height
    };

    for (int i = 0; i < wallCount; i++) {
        if (CheckCollisionRecs(proposedRectWall, walls[i])) {
            colidiuComParede = true;
            break;
        }
    }

    // Verifica colisão com as paredes INVISIVEIS
    bool colidiuComInvisible = false;
    Rectangle *invisibles = GetInvisible();
    int InvisibleCount = GetInvisibleCount();

    Vector2 proposedPositionINV = enemy->position;
    proposedPositionINV.x += enemy->velocity.x * dt;

    Texture2D currentTexInv = GetEnemyTexture(enemy, enemySprites);

    Rectangle proposedRectInvisible = {
        proposedPositionINV.x,
        enemy->position.y,
        (float)currentTex.width,
        (float)currentTex.height
    };

    for (int i = 0; i < InvisibleCount; i++) {
        if (CheckCollisionRecs(proposedRectInvisible, invisibles[i])) {
            colidiuComInvisible = true;
            break;
        }
    }

    if (!colidiuComInvisible) {
        enemy->position.x = proposedPositionINV.x;
    }

    // Verifica colisão com os tetos
    bool colidiuComTeto = false;
    Rectangle *floors = GetFloors();
    int floorCount = GetFloorCount();

    Vector2 proposedPositionY = enemy->position;
    proposedPositionY.y += enemy->velocity.y * dt;

    Rectangle proposedRectFloor = {
        enemy->position.x,
        proposedPositionY.y,
        (float)currentTex.width,
        (float)currentTex.height
    };

    for (int i = 0; i < floorCount; i++) {
        if (CheckCollisionRecs(proposedRectFloor, floors[i])) {
            colidiuComTeto = true;
            break;
        }
    }

    if (!colidiuComTeto) {
        enemy->position.y = proposedPositionY.y;
    } else {
        if (enemy->velocity.y < 0) enemy->velocity.y = 0;
    }

    Rectangle proposedRect = {proposedPosition.x, enemy->position.y, (float)currentTex.width, (float)currentTex.height};

    enemy->timer += dt;
    float frameTime;
    if (enemy->attacking) {
        frameTime = enemySprites.frame_change * enemy->attackVelocity;
    } else {
        frameTime = enemySprites.frame_change;
    }
    
    if (enemy->timer > frameTime) {
        enemy->timer = 0;
    
        if (enemy->attacking) {
            enemy->frame = (enemy->frame + 1) % enemySprites.attack_right.frame_count;
        } else {
            enemy->frame = (enemy->frame + 1) % enemySprites.walk_right.frame_count;
        }
    }

    if (enemy->hitTimer > 0) {
        enemy->hitTimer -= dt;
    }

    if (enemy->attacking && enemy->alive) {
        Rectangle attackArea = GetEnemyRect(enemy, currentTex);

    if (enemy->facing >= 0) {
        attackArea.width += 30; 
    } else {
        attackArea.x -= 30;
        attackArea.width += 30;
    }

}

}

Texture2D GetEnemyTexture(Enemy *enemy, EnemySprites enemySprites) {
    if (enemy->attacking) {
        return (enemy->facing >= 0) ?
            enemySprites.attack_right.frames[enemy->frame] :
            enemySprites.attack_left.frames[enemy->frame];
    } else {
        return (enemy->facing >= 0) ?
            enemySprites.walk_right.frames[enemy->frame] :
            enemySprites.walk_left.frames[enemy->frame];
    }
}

Rectangle GetEnemyRect(Enemy *enemy, Texture2D texture) {
    return (Rectangle){enemy->position.x, enemy->position.y, (float)texture.width, (float)texture.height};
}

void DrawEnemy(Enemy *enemy, Texture2D texture) {
    if (!enemy->alive) return;
    Color tint = (enemy->hitTimer > 0) ? RED : WHITE;
    DrawTexture(texture, (int)enemy->position.x, (int)enemy->position.y, tint);
}

void DamageEnemy(Enemy *enemy, int damage) {
    if (!enemy->alive) return;

    enemy->health -= damage;
    enemy->hitTimer = ENEMY_HIT_DURATION;

    if (enemy->health <= 0) {
        enemy->health = 0;
        enemy->alive = false;
    }
}