#include "map_original.h"
#include <stdlib.h>
#include <math.h>

static Rectangle ground;
static Texture2D groundTexture;

void InitMapOriginal(void) {
    ground = (Rectangle){0, 550, 2000, 500};

    groundTexture = LoadTexture("assets/sprites/map/ground/ground2.png");

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

}

Rectangle GetMapOriginalGround(void) {
    return ground;
}


void UnloadMapOriginal(void) {
    UnloadTexture(groundTexture);
}
