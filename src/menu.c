#include "raylib.h"
#include "menu.h"

GameScreen RunMenu() {
    const char *options[] = {"Jogar", "Sair"};
    int selected = 0;
    int optionCount = sizeof(options) / sizeof(options[0]);

    while (!WindowShouldClose()) {
        // Input
        if (IsKeyPressed(KEY_UP)) selected--;
        if (IsKeyPressed(KEY_DOWN)) selected++;
        if (selected < 0) selected = optionCount - 1;
        if (selected >= optionCount) selected = 0;

        if (IsKeyPressed(KEY_ENTER)) {
            if (selected == 0) return SCREEN_GAME;
            if (selected == 1) return SCREEN_EXIT;
        }

        // Draw
        BeginDrawing();
        ClearBackground(BLACK);

        DrawText("MENU PRINCIPAL", 280, 100, 30, WHITE);

        for (int i = 0; i < optionCount; i++) {
            Color color = (i == selected) ? YELLOW : GRAY;
            DrawText(options[i], 360, 200 + i * 40, 20, color);
        }

        EndDrawing();
    }

    return SCREEN_EXIT;
}
