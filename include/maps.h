#ifndef MAPS_H
#define MAPS_H

#include "raylib.h"

typedef enum {
    MAP_ORIGINAL,
    MAP_1
} MapType;

void DrawWall(int x, int baseY, int heightInPixels);
void CreateWallComColisao(int x, int baseY, int altura);
void UnloadWallTile(void);

Rectangle *GetWalls(void);
int GetWallCount(void);

#endif
