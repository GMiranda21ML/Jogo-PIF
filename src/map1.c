#include "map1.h"
#include "raylib.h"

Rectangle ground1;
Rectangle platforms1[MAP1_PLATFORM_COUNT];
Texture2D background1;

void InitMap1() {
    ground1 = (Rectangle){0, 550, 2000, 500};

    // Exemplo de plataformas
    platforms1[0] = (Rectangle){200, 450, 150, 20};
    platforms1[1] = (Rectangle){450, 350, 150, 20};
    platforms1[2] = (Rectangle){300, 250, 100, 20};
    platforms1[3] = (Rectangle){685, 250, 100, 20};

    // Carrega o background do mapa 1
    background1 = LoadTexture("assets/backgroundMap/backgroundForest2.png");
}

void DrawMap1(Texture2D groundTile) {
    DrawTexture(background1, 0, -490, WHITE); // Ajuste Y conforme necessário

    int tileWidth = groundTile.width;
    int tiles = ground1.width / tileWidth;
    for (int i = 0; i < tiles + 1; i++) {
        DrawTexture(groundTile, ground1.x + i * tileWidth, ground1.y, WHITE);
    }

    for (int i = 0; i < MAP1_PLATFORM_COUNT; i++) {
        DrawRectangleRec(platforms1[i], GRAY);
    }
}

void UnloadMap1() {
    UnloadTexture(background1);
}
