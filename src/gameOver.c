#include "raylib.h"
#include "menu.h"

GameScreen RunGameOver() {
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);

        const char *text = "Game Over";
        int fontSize = 50;
        int x = GetScreenWidth()/2 - MeasureText(text, fontSize)/2;
        int y = GetScreenHeight()/2 - fontSize;
        DrawText(text, x + 3, y + 3, fontSize + 2, DARKGRAY);
        DrawText(text, x, y, fontSize, RED);

        const char *retry = "Pressione ENTER para voltar ao menu";
        int retryFontSize = 20;
        DrawText(retry, GetScreenWidth()/2 - MeasureText(retry, retryFontSize)/2, y + 100, retryFontSize, WHITE);

        EndDrawing();

        if (IsKeyPressed(KEY_ENTER)) {
            return SCREEN_MENU;
        }
    }

    return SCREEN_EXIT;
}
