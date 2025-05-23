#include "raylib.h"
#include "screens.h"

GameScreen RunEpilogueCutscene(float totalGameTime) {
    const char *imagePaths[NUM_IMAGES] = {
        "assets/backgroundsEpilogueCutScene/epilogueImage1.png",
        "assets/backgroundsEpilogueCutScene/epilogueImage3.png",
        "assets/backgroundsEpilogueCutScene/epilogueImage2.png"
    };

    Dialogue dialogue1 = { "Após a grande jornada até chegar na princesa...", 3.0f };
    Dialogue dialogue2 = { "O herói finalmente encontrou a sua amada...", 4.5f };
    Dialogue dialogue3 = { "Juntos, escaparam daquele castelo...", 4.0f };
    Dialogue dialogue4 = { "E partiram para um novo destino...", 4.0f };
    Dialogue dialogue5 = { "O amor venceu a escuridão naquele dia...", 6.0f };

    Dialogue dialogues[NUM_DIALOGUES] = {
        dialogue1, dialogue2, dialogue3, dialogue4, dialogue5
    };

    Texture2D images[NUM_IMAGES];
    for (int i = 0; i < NUM_IMAGES; i++) {
        images[i] = LoadTexture(imagePaths[i]);
    }

    Music music = LoadMusicStream("assets/sound/cutSceneMusic/endGame.mp3");
    PlayMusicStream(music);

    float imageTimer = 0.0f;
    float textTimer = 0.0f;
    int currentImage = 0;
    int currentText = 0;
    bool skip = false;
    bool transitioning = false;
    bool fadingOut = false;
    float fadeTimer = 0.0f;

    while (!WindowShouldClose() && !skip) {
        float dt = GetFrameTime();
        imageTimer += dt;
        textTimer += dt;

        UpdateMusicStream(music);

        if (IsKeyPressed(KEY_SPACE)) {
            skip = true;
        } 

        if (!transitioning && !fadingOut && imageTimer >= IMAGE_DISPLAY_TIME && currentImage < NUM_IMAGES - 1) {
            transitioning = true;
            imageTimer = 0.0f;
        }

        if (transitioning && imageTimer >= TRANSITION_TIME) {
            transitioning = false;
            currentImage++;
            imageTimer = 0.0f;
        }

        if (currentText < NUM_DIALOGUES && textTimer >= dialogues[currentText].displayTime) {
            currentText++;
            textTimer = 0.0f;
        }

        if (!fadingOut && currentText >= NUM_DIALOGUES && currentImage == NUM_IMAGES - 1) {
            fadingOut = true;
            fadeTimer = 0.0f;
        }

        if (fadingOut) {
            fadeTimer += dt;
            if (fadeTimer >= FADE_OUT_TIME) {
                skip = true;
            }
        }

        float alphaCurrent = 255.0f;
        float alphaNext = 0.0f;

        if (transitioning) {
            float t = imageTimer / TRANSITION_TIME;
            alphaCurrent = (1.0f - t) * 255.0f;
            alphaNext = t * 255.0f;
        }

        BeginDrawing();

        if (fadingOut) {
            ClearBackground(BLACK);

            float scale = 0.7f;
            Vector2 imageSize = { images[currentImage].width * scale, images[currentImage].height * scale };
            Vector2 pos = { GetScreenWidth()/2 - imageSize.x/2, GetScreenHeight()/2 - imageSize.y/2 };

            DrawTextureEx(images[currentImage], pos, 0.0f, scale, WHITE);

            if (currentText < NUM_DIALOGUES) {
                const char *line = dialogues[currentText].text;
                int textX = GetScreenWidth()/2 - MeasureText(line, 28)/2;
                int textY = GetScreenHeight() - 100;

                DrawText(line, textX + 2, textY + 2, 28, BLACK);
                DrawText(line, textX, textY, 28, WHITE);
            }

            float fadeAlpha = (fadeTimer / FADE_OUT_TIME) * 255.0f;
            DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), (Color){ 0, 0, 0, (unsigned char)fadeAlpha });
        }
        else {
            ClearBackground(BLACK);

            float scale = 0.7f;
            Vector2 imageSize = { images[currentImage].width * scale, images[currentImage].height * scale };
            Vector2 pos = { GetScreenWidth()/2 - imageSize.x/2, GetScreenHeight()/2 - imageSize.y/2 };

            DrawTextureEx(images[currentImage], pos, 0.0f, scale, (Color){ 255, 255, 255, (unsigned char)alphaCurrent });

            if (transitioning && currentImage < NUM_IMAGES - 1) {
                Texture2D nextTex = images[currentImage + 1];
                DrawTextureEx(nextTex, pos, 0.0f, scale, (Color){ 255, 255, 255, (unsigned char)alphaNext });
            }

            if (currentText < NUM_DIALOGUES) {
                const char *line = dialogues[currentText].text;
                int textX = GetScreenWidth()/2 - MeasureText(line, 28)/2;
                int textY = GetScreenHeight() - 100;

                DrawText(line, textX + 2, textY + 2, 28, BLACK);
                DrawText(line, textX, textY, 28, WHITE);
            }

            DrawText("Pressione ESPAÇO para pular", 240, 550, 20, GRAY);
        }

        EndDrawing();
    }

    for (int i = 0; i < NUM_IMAGES; i++) {
        UnloadTexture(images[i]);
    }

    return ShowEndGameScreen(&music, totalGameTime);
}
