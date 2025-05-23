#include "map_original.h"
#include <stdlib.h>
#include <math.h>

#define ORIGINAL_MAP_PLATFORM_COUNT 0

static Rectangle originalPlatforms[ORIGINAL_MAP_PLATFORM_COUNT];
static Rectangle ground;
static Texture2D groundTexture;

void InitMapOriginal(void) {
    originalPlatforms[0] = (Rectangle){200, 450, 150, 20};
    originalPlatforms[1] = (Rectangle){450, 350, 150, 20};
    originalPlatforms[2] = (Rectangle){300, 250, 100, 20};
    originalPlatforms[3] = (Rectangle){685, 250, 100, 20};

    ground = (Rectangle){0, 550, 2000, 500};

    groundTexture = LoadTexture("assets/sprites/map/ground/ground1.png");

}
void DrawMapOriginal(void) {
    int tileWidth = groundTexture.width;
    int tileHeight = groundTexture.height;

    int tilesX = (int)ceil(ground.width / (float)tileWidth);
    int tilesY = (int)ceil(ground.height / (float)tileHeight);

    for (int y = 0; y < tilesY; y++) {
        for (int x = 0; x < tilesX; x++) {
            DrawTexture(groundTexture, ground.x + x * tileWidth, ground.y + y * tileHeight, WHITE);
        }
    }

    for (int i = 0; i < ORIGINAL_MAP_PLATFORM_COUNT; i++) {
        DrawRectangleRec(originalPlatforms[i], GRAY);
    }
}

Rectangle* GetMapOriginalPlatforms(void) {
    return originalPlatforms;
}

int GetMapOriginalPlatformCount(void) {
    return ORIGINAL_MAP_PLATFORM_COUNT;
}

Rectangle GetMapOriginalGround(void) {
    return ground;
}


void UnloadMapOriginal(void) {
    UnloadTexture(groundTexture);
}
