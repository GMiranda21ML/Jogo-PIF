#include "chave.h"
#include <stdio.h>

// global.c ou game_state.c
bool chaveFoiColetada = false;

// no seu código onde carrega o mapa:
void InitChave(Chave *chave, Vector2 pos, Texture2D textura) {
    chave->pos = pos;
    chave->tamanho = (float)textura.width;
    chave->ativa = !chaveFoiColetada;
    chave->textura = textura;
}

void UpdateChave(Chave *chave, Player *player) {
    if (chave->ativa) {
        Rectangle chaveRec = { chave->pos.x, chave->pos.y, chave->tamanho, chave->tamanho };
        Rectangle playerRec = GetPlayerRect(player);

        if (CheckCollisionRecs(playerRec, chaveRec)) {
            chave->ativa = false;
            chaveFoiColetada = true; // <- Salva o estado da coleta
            player->PossuiKey = true;
        }
    }
}

void ResetarChaveColetada() {
    chaveFoiColetada = false;
}

void DrawChave(Chave *chave) {
    if (chave->ativa) {
        DrawTexture(chave->textura, (int)chave->pos.x, (int)chave->pos.y, WHITE);
    }
}

bool ChaveColetada(Chave *chave) {
    return !chave->ativa;
}

void UnloadChave(Chave *chave) {
    UnloadTexture(chave->textura);
}