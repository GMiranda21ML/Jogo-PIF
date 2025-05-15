#include "raylib.h"
#include "screens.h"

GameScreen RunCutscene() {
    Music music = LoadMusicStream("assets/sound/cutSceneMusic/music.mp3");
    PlayMusicStream(music);

    float cutsceneTimer = 0.0f;
    bool skip = false;

    while (!WindowShouldClose() && !skip) {
        float dt = GetFrameTime();
        cutsceneTimer += dt;

        UpdateMusicStream(music);

        if (IsKeyPressed(KEY_SPACE)) {
            skip = true;
        }

        BeginDrawing();
        ClearBackground(BLACK);

        if (cutsceneTimer < 2.0f) {
            DrawText("Um grande heroi recebeu uma missão...", 90, 250, 30, WHITE);
        } else if (cutsceneTimer < 5.0f) {
            DrawText("De salvar a princesa do castelo sombrio", 75, 250, 30, WHITE);
        } else if (cutsceneTimer < 8.0f) {
            DrawText("O herói solitário caminha pelas ruínas...", 75, 250, 30, WHITE);
        } else if (cutsceneTimer < 11.0f) {
            DrawText("Será que ele vai conseguir salvar a princesa?", 45, 250, 30, WHITE);
        } else {
            skip = true;
        }

        DrawText("Pressione ESPAÇO para pular", 240, 550, 20, GRAY);
        EndDrawing();
    }

    StopMusicStream(music);
    UnloadMusicStream(music);

    return SCREEN_MENU;
}
