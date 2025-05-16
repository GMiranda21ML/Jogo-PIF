#include "raylib.h"
#include "screens.h"

GameScreen RunCutscene() {
    Texture2D image = LoadTexture("assets/backgroundCutScene/cutSceneBackground.png");
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

        int brightness = (int)((cutsceneTimer / 10.5f) * 255.0f);
        if (brightness > 255) {
            brightness = 255;
        }

        Color tint = (Color){brightness, brightness, brightness, 255};

        BeginDrawing();
        ClearBackground(BLACK);

        float scale = 0.9f;
        Vector2 imageSize = { image.width * scale, image.height * scale };
        Vector2 imagePos = { GetScreenWidth()/2 - imageSize.x/2, GetScreenHeight()/2 - imageSize.y/2 };

        DrawTextureEx(image, imagePos, 0.0f, scale, tint);

        if (cutsceneTimer < 2.0f) {
            DrawText("Um grande heroi recebeu uma missão...", 95, 255, 30, BLACK);
            DrawText("Um grande heroi recebeu uma missão...", 90, 250, 30, WHITE);
        } else if (cutsceneTimer < 5.0f) {
            DrawText("De salvar a princesa do castelo sombrio", 80, 255, 30, BLACK);
            DrawText("De salvar a princesa do castelo sombrio", 75, 250, 30, WHITE);
        } else if (cutsceneTimer < 8.0f) {
            DrawText("O herói solitário caminha pelas ruínas...", 80, 255, 30, BLACK);
            DrawText("O herói solitário caminha pelas ruínas...", 75, 250, 30, WHITE);
        } else if (cutsceneTimer < 11.0f) {
            DrawText("Será que ele vai conseguir salvar a princesa?", 50, 255, 30, BLACK);
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
