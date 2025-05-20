#include "map3.h"
#include "maps.h"
#include "raylib.h"

Rectangle ground3;
Rectangle platforms3[MAP3_PLATFORM_COUNT];

static Texture2D background3;
static Texture2D groundTile3;

void InitMap3() {
    ground3 = (Rectangle){350, 550, 2000, 10};

    background3 = LoadTexture("assets/backgroundMap/backgroundCastle2.png");
    groundTile3 = LoadTexture("assets/sprites/map/ground/groundCastle.png");
    Texture2D wallTile = LoadTexture("assets/sprites/map/wall/wallCastle.png");
    Texture2D ceilingTile = LoadTexture("assets/sprites/map/ground/groundCastle.png");
    LoadSpikeTexture();
    
    SetWallTile(wallTile);
    SetCeilingTile(ceilingTile);
}

void DrawMap3() {
    DrawTexture(background3, 0, -490, WHITE); 

    int tileWidth = groundTile3.width;
    int tiles = ground3.width / tileWidth;
    for (int i = 0; i < tiles + 1; i++) {
        DrawTexture(groundTile3, ground3.x + i * tileWidth, ground3.y, WHITE);
    }

    for (int i = 0; i < MAP3_PLATFORM_COUNT; i++) {
        DrawRectangleRec(platforms3[i], GRAY);
    }


    CreateCeilingComColisao(0, 2000, 0, 10);
    CreateCeilingComColisao(0, 250, 550, 10);
}

Rectangle* GetMap3Platforms() {
    return platforms3;
}

Rectangle GetMap3Ground(void) {
    return ground3;
}

int GetMap3PlatformCount(void) {
    return MAP3_PLATFORM_COUNT;
}

void UnloadMap3() {
    UnloadTexture(background3);
    UnloadTexture(groundTile3);
    UnloadSpikeTexture();
    UnloadWallTile();
}
