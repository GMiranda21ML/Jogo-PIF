#ifndef CHAVE_H
#define CHAVE_H

#include "raylib.h"
#include <stdbool.h>
#include "player.h"

typedef struct {
    Vector2 pos;
    float tamanho;
    bool ativa;
    Texture2D textura;
} Chave;

void InitChave(Chave *chave, Vector2 pos, Texture2D Texture);
void UpdateChave(Chave *chave, Player *player);
void ResetarChaveColetada();
void DrawChave(Chave *chave);
bool ChaveColetada(Chave *chave);
void UnloadChave(Chave *chave);

#endif 
