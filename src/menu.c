#include "raylib.h"
#include "screens.h"

GameScreen RunMenu(Music menuMusic) {
    const char options[4][10] = {
        "Jogar",
        "Teclas",
        "Score",
        "Sair"
    };

    int selected = 0;
    int optionCount = sizeof(options) / sizeof(options[0]);

    Texture2D logo = LoadTexture("assets/sprites/menu/menu2.png");
    Vector2 logoPosition;

    logoPosition.x = (float)(GetScreenWidth() - logo.width) / 2;
    logoPosition.y = (float)(GetScreenHeight() - logo.height) / 2;

    while (!WindowShouldClose()) {
        UpdateMusicStream(menuMusic);

        if (IsKeyPressed(KEY_UP)) {
            selected = selected - 1;
        }
        if (IsKeyPressed(KEY_DOWN)) {
            selected = selected + 1;
        }
        if (selected < 0) {
            selected = optionCount - 1;
        }
        if (selected >= optionCount) {
            selected = 0;
        }

        if (IsKeyPressed(KEY_ENTER)) {
            if (selected == 0) {
                return SCREEN_GAME;
            }
            if (selected == 1) {
                return SCREEN_KEYBOARD;
            }
            if (selected == 2) {
                return SCREEN_SCORE;
            }
            if (selected == 3) {
                return SCREEN_EXIT;
            }
        }

        BeginDrawing();
        ClearBackground(BLACK);

        DrawTexture(logo, logoPosition.x, logoPosition.y, WHITE);

        const char *title = "Metroid Souls";
        int titleFontSize = 60;
        int titleX;
        int titleY;

        titleX = GetScreenWidth() / 2 - MeasureText(title, titleFontSize) / 2;
        titleY = 100;

        DrawText(title, titleX + 3, titleY + 3, titleFontSize + 2, BLACK);
        DrawText(title, titleX, titleY, titleFontSize, WHITE);

        for (int i = 0; i < 4; i++) {
            const char *text = options[i];
            int fontSize = 30;
            int spacing = 50;
            int yPos = titleY + 100 + i * spacing;
            int textX = GetScreenWidth() / 2 - MeasureText(text, fontSize) / 2;

            if (i == selected) {
                DrawText(text, textX + 2, yPos + 2, fontSize + 2, BLACK);
                DrawText(text, textX, yPos, fontSize, YELLOW);
            } else {
                DrawText(text, textX, yPos, fontSize, GRAY);
            }
        }

        EndDrawing();
    }

    UnloadTexture(logo);

    return SCREEN_EXIT;
}
