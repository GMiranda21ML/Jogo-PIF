#include "raylib.h"
#include "screens.h"

GameScreen RunKeyboardScreen(Music menuMusic) {
    while (!WindowShouldClose()) {
        UpdateMusicStream(menuMusic);
        if (IsKeyPressed(KEY_SPACE)) {
            return SCREEN_MENU;
        }

        BeginDrawing();
        ClearBackground(BLACK);

        const char *title = "Controles";
        int titleFontSize = 50;
        int titleX = GetScreenWidth() / 2 - MeasureText(title, titleFontSize) / 2;
        int titleY = 50;

        DrawText(title, titleX + 2, titleY + 2, titleFontSize, DARKGRAY);
        DrawText(title, titleX, titleY, titleFontSize, GOLD);

        int fontSize = 30;
        int y = 150;

        DrawText("W - Pular", 100, y, fontSize, GRAY);
        DrawText("A - Andar para a esquerda", 100, y + 40, fontSize, GRAY);
        DrawText("D - Andar para a direita", 100, y + 80, fontSize, GRAY);
        DrawText("L - Atacar", 100, y + 120, fontSize, GRAY);
        DrawText("E - Interagir com objetos", 100, y + 160, fontSize, GRAY);
        DrawText("R - Reiniciar (voltar ao menu)", 100, y + 200, fontSize, GRAY);
        DrawText("ENTER - Confirmar", 100, y + 240, fontSize, GRAY);

        DrawText("Aperte SPACE para voltar ao menu", 100, y + 350, fontSize, WHITE);

        EndDrawing();
    }

    return SCREEN_EXIT;
}
