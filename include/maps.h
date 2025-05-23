#ifndef MAPS_H
#define MAPS_H

#include "raylib.h"

typedef enum {
    MAP_ORIGINAL,
    MAP_1,
    MAP_2,
    MAP_3
} MapType;

#define MAX_WALLS 1000
#define MAX_INVISIBLE 1000
#define MAX_FLOORS 1000
#define MAX_SPIKES 100

void DrawInvisible(int x, int baseY, int heightInPixels);
void CreateInvisibleComColisao(int x, int baseY, int altura);
void DrawWall(int x, int baseY, int heightInPixels);
void CreateWallComColisao(int x, int baseY, int altura);
void DrawCeiling(int xInicio, int xFim, int topY, int altura);
void CreateFloorComColisao(int xInicio, int xFim, int topY, int altura);
void ClearInvisibleCollision(void);
void ClearWallCollision(void);
void ClearFloorCollision(void);
void ClearAllMapCollisions(void);
void ClearSpikeCollision(void);
void UnloadInvisibleTile(void);
void UnloadWallTile(void);
void UnloadFloorTile(void);
void CreateSpike(int xInicio, int yInicio, int xFim, int yFim);
void DrawSpikes(void);
void CheckSpikeDamage(Rectangle playerRect, int *playerHealth);
void LoadSpikeTexture(void);
void UnloadSpikeTexture(void);

Rectangle *GetWalls(void);
int GetWallCount(void);

Rectangle *GetInvisible(void);
int GetInvisibleCount(void);

Rectangle *GetFloors(void);
int GetFloorCount(void);

#endif
