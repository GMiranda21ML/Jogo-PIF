#include "raylib.h"
#include "screens.h"

#define NUM_IMAGES 3
#define NUM_DIALOGUES 5
#define TRANSITION_TIME 3.0f
#define IMAGE_DISPLAY_TIME 6.0f

typedef struct {
    const char *text;
    float displayTime;
} Dialogue;

GameScreen RunEpilogueCutscene() {
    const char *imagePaths[NUM_IMAGES] = {
        "assets/backgroundsEpilogueCutScene/epilogueImage1.png",
        "assets/backgroundsEpilogueCutScene/epilogueImage3.png",
        "assets/backgroundsEpilogueCutScene/epilogueImage2.png"
    };

    // Diálogos individuais com tempos personalizados
    Dialogue dialogue1 = { "Após a grande jornada até chegar na princesa...", 3.0f };
    Dialogue dialogue2 = { "O herói finalmente encontrou a sua amada...", 4.5f };
    Dialogue dialogue3 = { "Juntos, escaparam daquele castelo...", 3.0f };
    Dialogue dialogue4 = { "E partiram para um novo destino...", 5.0f };
    Dialogue dialogue5 = { "O amor venceu a escuridão naquele dia...", 6.0f };

    Dialogue dialogues[NUM_DIALOGUES] = {
        dialogue1, dialogue2, dialogue3, dialogue4, dialogue5
    };

    Texture2D images[NUM_IMAGES];
    for (int i = 0; i < NUM_IMAGES; i++) {
        images[i] = LoadTexture(imagePaths[i]);
    }

    Music music = LoadMusicStream("assets/sound/cutSceneMusic/final.mp3");
    PlayMusicStream(music);

    float imageTimer = 0.0f;
    float textTimer = 0.0f;
    int currentImage = 0;
    int currentText = 0;
    bool skip = false;
    bool transitioning = false;

    while (!WindowShouldClose() && !skip) {
        float dt = GetFrameTime();
        imageTimer += dt;
        textTimer += dt;

        UpdateMusicStream(music);

        if (IsKeyPressed(KEY_SPACE)) skip = true;

        // Transição de imagem
        if (!transitioning && imageTimer >= IMAGE_DISPLAY_TIME && currentImage < NUM_IMAGES - 1) {
            transitioning = true;
            imageTimer = 0.0f;
        }

        if (transitioning && imageTimer >= TRANSITION_TIME) {
            transitioning = false;
            currentImage++;
            imageTimer = 0.0f;
        }

        // Avança fala de acordo com tempo individual
        if (currentText < NUM_DIALOGUES && textTimer >= dialogues[currentText].displayTime) {
            currentText++;
            textTimer = 0.0f;
        }

        float alphaCurrent = 255.0f;
        float alphaNext = 0.0f;

        if (transitioning) {
            float t = imageTimer / TRANSITION_TIME;
            alphaCurrent = (1.0f - t) * 255.0f;
            alphaNext = t * 255.0f;
        }

        BeginDrawing();
        ClearBackground(BLACK);

        float scale = 0.7f;
        Vector2 imageSize = { images[currentImage].width * scale, images[currentImage].height * scale };
        Vector2 pos = { GetScreenWidth()/2 - imageSize.x/2, GetScreenHeight()/2 - imageSize.y/2 };

        DrawTextureEx(images[currentImage], pos, 0.0f, scale, (Color){255, 255, 255, (unsigned char)alphaCurrent});

        if (transitioning && currentImage < NUM_IMAGES - 1) {
            Texture2D nextTex = images[currentImage + 1];
            DrawTextureEx(nextTex, pos, 0.0f, scale, (Color){255, 255, 255, (unsigned char)alphaNext});
        }

        // Desenhar texto atual com sombra, se ainda houver
        if (currentText < NUM_DIALOGUES) {
            const char *line = dialogues[currentText].text;
            int textX = GetScreenWidth()/2 - MeasureText(line, 28)/2;
            int textY = GetScreenHeight() - 100;

            DrawText(line, textX + 2, textY + 2, 28, BLACK);  // Sombra
            DrawText(line, textX, textY, 28, WHITE);
        }

        DrawText("Pressione ESPAÇO para pular", 240, 550, 20, GRAY);
        EndDrawing();

        if (currentText >= NUM_DIALOGUES) {
            skip = true;
        }
    }

    StopMusicStream(music);
    UnloadMusicStream(music);
    for (int i = 0; i < NUM_IMAGES; i++) {
        UnloadTexture(images[i]);
    }

    return ShowEndGameScreen();
}
