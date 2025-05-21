#include "screens.h"
#include "raylib.h"
#include <stdbool.h>
#include <stdio.h>

GameScreen RunBestTimeScreen(Music menuMusic) {
    float bestTime = 0.0f;
    FILE *file = fopen(BEST_TIME_FILE, "r");
    if (file) {
        fscanf(file, "%f", &bestTime);
        fclose(file);
    }

    bool exitScreen = false;

    while (!WindowShouldClose() && !exitScreen) {
        UpdateMusicStream(menuMusic);
        if (IsKeyPressed(KEY_SPACE)) {
            exitScreen = true;
        }

        BeginDrawing();
        ClearBackground(BLACK);

        const char *title = "Melhor Tempo do Jogo";
        DrawText(title, GetScreenWidth()/2 - MeasureText(title, 40)/2, 100, 40, GOLD);

        char timeText[50];
        sprintf(timeText, "%.2f segundos", bestTime);
        DrawText(timeText, GetScreenWidth()/2 - MeasureText(timeText, 30)/2, 250, 30, WHITE);

        DrawText("Pressione SPACE para voltar", GetScreenWidth()/2 - MeasureText("Pressione SPACE para voltar", 20)/2, 500, 20, GRAY);

        EndDrawing();
    }

    return SCREEN_MENU; 
}
