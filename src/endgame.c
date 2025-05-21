#include "raylib.h"
#include "screens.h"

GameScreen ShowEndGameScreen(void) {
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);

        const char *title = "FIM DE JOGO";
        const char *prompt = "Aperte BACKSPACE para voltar ao menu";

        int titleFontSize = 60;
        int promptFontSize = 24;

        int titleWidth = MeasureText(title, titleFontSize);
        int promptWidth = MeasureText(prompt, promptFontSize);

        DrawText(title, GetScreenWidth()/2 - titleWidth/2, GetScreenHeight()/2 - 60, titleFontSize, WHITE);
        DrawText(prompt, GetScreenWidth()/2 - promptWidth/2, GetScreenHeight()/2 + 10, promptFontSize, GRAY);

        EndDrawing();

        if (IsKeyPressed(KEY_BACKSPACE)) {
            break;
        }
    }

    return SCREEN_MENU;
}
