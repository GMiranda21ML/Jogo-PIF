#include "porta.h"
#include "raylib.h"

void InitPorta(Porta *porta, Vector2 pos, Texture2D texturaFechada, Texture2D texturaAberta) {
    porta->pos = pos;
    porta->tamanho = (float)texturaFechada.width;
    porta->aberta = false;
    porta->texturaFechada = texturaFechada;
    porta->texturaAberta = texturaAberta;
}

void UpdatePorta(Porta *porta, Player *player) {
    Rectangle portaRec = { porta->pos.x, porta->pos.y, porta->tamanho, porta->tamanho };
    Rectangle playerRec = GetPlayerRect(player);

    if (CheckCollisionRecs(playerRec, portaRec) && player->PossuiKey) {
        if (IsKeyPressed(KEY_E)) {
            porta->aberta = true;
        }
    }
}

void DrawPorta(Porta *porta) {
    if (porta->aberta) {
        DrawTexture(porta->texturaAberta, (int)porta->pos.x, (int)porta->pos.y, WHITE);
    } else {
        DrawTexture(porta->texturaFechada, (int)porta->pos.x, (int)porta->pos.y, WHITE);
    }
}

void UnloadPorta(Porta *porta) {
    UnloadTexture(porta->texturaFechada);
    UnloadTexture(porta->texturaAberta);
}

bool PortaAberta(Porta *porta) {
    return porta->aberta;
}
