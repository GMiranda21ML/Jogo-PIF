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
    enemy->attacking = false;
    enemy->facing = 1;
}

void UpdateEnemy(Enemy *enemy, Vector2 playerPos, float dt, EnemySprites skeleton, Rectangle playerRect) {
    if (!enemy->alive) return;

    enemy->position.y = 550 - skeleton.walk_right.frames[0].height;

    float deltaX = enemy->position.x - playerPos.x;
    float deltaY = enemy->position.y - playerPos.y;
    float xDistance = sqrt(deltaX * deltaX + deltaY * deltaY);
    bool detected = xDistance < DETECTION_RADIUS;
    bool tooClose = xDistance < MIN_DISTANCE_TO_PLAYER;

    if (detected){
        float sign = playerPos.x - enemy->position.x < 0 ? -1 : 1;
        enemy->velocity.x = sign * ENEMY_VELOCITY;
        enemy->facing = sign;
        enemy->attacking = tooClose;
    }

    Vector2 proposedPosition = enemy->position;
    proposedPosition.x += enemy->velocity.x * dt;

    Texture2D currentTex = GetEnemyTexture(enemy, skeleton);
    Rectangle proposedRect = {proposedPosition.x, enemy->position.y, (float)currentTex.width, (float)currentTex.height};

    if (!CheckCollisionRecs(proposedRect, playerRect)) {
        enemy->position.x = proposedPosition.x;
    } else {
        enemy->velocity.x = 0;
    }

    enemy->timer += dt;
    if (enemy->timer > skeleton.frame_change) {
        enemy->timer = 0;
        if (enemy->attacking) {
            enemy->frame = (enemy->frame + 1) % skeleton.attack_right.frame_count;
        } else {
            enemy->frame = (enemy->frame + 1) % skeleton.walk_right.frame_count;
        }
    }

    if (enemy->hitTimer > 0) {
        enemy->hitTimer -= dt;
    }

    // Aplica dano ao jogador
    if (enemy->attacking && enemy->alive) {
        Rectangle attackArea = GetEnemyRect(enemy, currentTex);

    // Aumentar alcance para frente
    if (enemy->facing >= 0) {
        attackArea.width += 30; // aumenta o alcance para frente
    } else {
        attackArea.x -= 30;
        attackArea.width += 30;
    }

    // Verifica colisão com jogador
    if (CheckCollisionRecs(attackArea, playerRect)) {
        // Marcar dano no jogador via variável global ou ponteiro
        // Aqui, você não pode aplicar diretamente, mas avisar o main que o jogador foi atingido.
        // Você pode usar uma flag como retorno ou sinalizar dano de outra forma
        // Como alternativa temporária, deixe esse trecho vazio e a lógica fica no main.c
    }
}

}

Texture2D GetEnemyTexture(Enemy *enemy, EnemySprites skeleton) {
    if (enemy->attacking) {
        return (enemy->facing >= 0) ?
            skeleton.attack_right.frames[enemy->frame] :
            skeleton.attack_left.frames[enemy->frame];
    } else {
        return (enemy->facing >= 0) ?
            skeleton.walk_right.frames[enemy->frame] :
            skeleton.walk_left.frames[enemy->frame];
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

void DamageEnemy(Enemy *enemy) {
    enemy->health--;
    enemy->hitTimer = ENEMY_HIT_DURATION;
    if (enemy->health <= 0) {
        enemy->alive = false;
    }
}
