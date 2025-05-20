#include "map2.h"
#include "maps.h"
#include "raylib.h"

Rectangle ground2;
Rectangle platforms2[MAP2_PLATFORM_COUNT];

static Texture2D background2;
static Texture2D groundTile2;

void InitMap2() {
    ground2 = (Rectangle){0, 550, 2000, 500};

    background2 = LoadTexture("assets/backgroundMap/backgroundCastle2.png");
    groundTile2 = LoadTexture("assets/sprites/map/ground/groundCastle.png");
    Texture2D wallTile = LoadTexture("assets/sprites/map/wall/wallCastle.png");
    Texture2D ceilingTile = LoadTexture("assets/sprites/map/ground/groundCastle.png");

    SetWallTile(wallTile);
    SetCeilingTile(ceilingTile);
}

void DrawMap2() {
    DrawTexture(background2, 0, -490, WHITE); 

    int tileWidth = groundTile2.width;
    int tiles = ground2.width / tileWidth;
    for (int i = 0; i < tiles + 1; i++) {
        DrawTexture(groundTile2, ground2.x + i * tileWidth, ground2.y, WHITE);
    }

    for (int i = 0; i < MAP2_PLATFORM_COUNT; i++) {
        DrawRectangleRec(platforms2[i], GRAY);
    }

    CreateCeilingComColisao(0, 2000, 100,10);

    CreateCeilingComColisao(0, 250, 0,10);
    CreateCeilingComColisao(350, 1650, 0,10);
    CreateCeilingComColisao(1800, 2000, 0,10);
}

Rectangle* GetMap2Platforms() {
    return platforms2;
}

Rectangle GetMap2Ground(void) {
    return ground2;
}

int GetMap2PlatformCount(void) {
    return MAP2_PLATFORM_COUNT;
}

void UnloadMap2() {
    UnloadTexture(background2);
    UnloadTexture(groundTile2);
    UnloadSpikeTexture();
    UnloadWallTile();
}
