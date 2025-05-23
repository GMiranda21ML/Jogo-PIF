#ifndef PORTA_H
#define PORTA_H

#include "raylib.h"
#include "player.h"

typedef struct Porta {
    Vector2 pos;
    float tamanho;
    bool aberta;
    Texture2D texturaFechada;
    Texture2D texturaAberta;
} Porta;

void InitPorta(Porta *porta, Vector2 pos, Texture2D texturaFechada, Texture2D texturaAberta);
void UpdatePorta(Porta *porta, Player *player);
void DrawPorta(Porta *porta);
void UnloadPorta(Porta *porta);

bool PortaAberta(Porta *porta);

#endif
