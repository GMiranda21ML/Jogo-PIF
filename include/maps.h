#ifndef MAPS_H
#define MAPS_H

#include "raylib.h"

typedef enum {
    MAP_ORIGINAL,
    MAP_1,
    MAP_2,
    MAP_3
} MapType;

void DrawWall(int x, int baseY, int heightInPixels);
void CreateWallComColisao(int x, int baseY, int altura);
void DrawCeiling(int xInicio, int xFim, int topY, int altura);
void CreateCeilingComColisao(int xInicio, int xFim, int topY, int altura);
void ClearWallCollision(void);
void ClearCeilingCollision(void);
void ClearAllMapCollisions(void);
void ClearSpikeCollision(void);
void UnloadWallTile(void);
void UnloadCeilingTile(void);
void CreateSpike(int xInicio, int yInicio, int xFim, int yFim);
void DrawSpikes(void);
void CheckSpikeDamage(Rectangle playerRect, int *playerHealth);
void LoadSpikeTexture(void);
void UnloadSpikeTexture(void);

Rectangle *GetWalls(void);
int GetWallCount(void);

Rectangle *GetCeilings(void);
int GetCeilingCount(void);

#endif
