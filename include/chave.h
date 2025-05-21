#ifndef CHAVE_H
#define CHAVE_H

#include "raylib.h"
#include <stdbool.h>

typedef struct {
    Vector2 position;
    Texture2D texture;
    bool ativa;
} Chave;

bool CheckKeyCollected(Chave *chave, Rectangle playerRect);
void InitChave(Chave *chave, Vector2 pos, const char *path);
void DrawChave(Chave *chave);
void UpdateChave(Chave *chave, Rectangle playerRect, bool *possuiKey);
void UnloadChave(Chave *chave);

#endif
