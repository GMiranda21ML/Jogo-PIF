#include "chave.h"

void InitChave(Chave *chave, Vector2 pos, const char *path) {
    chave->position = pos;
    chave->texture = LoadTexture(path);
    chave->ativa = true;
}

void DrawChave(Chave *chave) {
    if (chave->ativa) {
        DrawTexture(chave->texture, (int)chave->position.x, (int)chave->position.y, WHITE);
    }
}

void UpdateChave(Chave *chave, Rectangle playerRect, bool *possuiKey) {
    if (chave->ativa) {
        Rectangle chaveRect = {
            chave->position.x, chave->position.y,
            (float)chave->texture.width, (float)chave->texture.height
        };

        if (CheckCollisionRecs(playerRect, chaveRect)) {
            *possuiKey = true;
            chave->ativa = false;
        }
    }
}

bool CheckKeyCollected(Chave *chave, Rectangle playerRect) {
    Rectangle chaveRect = {
        chave->position.x, chave->position.y,
        (float)chave->texture.width, (float)chave->texture.height
    };

    return !chave->ativa && CheckCollisionRecs(playerRect, chaveRect);
}

void UnloadChave(Chave *chave) {
    UnloadTexture(chave->texture);
}
