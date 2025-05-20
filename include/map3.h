#ifndef MAP3_H
#define MAP3_H

#include "raylib.h"
#include "maps.h"

#define MAP3_PLATFORM_COUNT 5

extern Rectangle ground3;
extern Rectangle platforms3[MAP3_PLATFORM_COUNT];

void InitMap3();
void DrawMap3();
void UnloadMap3();

Rectangle* GetMap3Platforms(void);
Rectangle GetMap3Ground(void);

int GetMap3PlatformCount(void);

#endif
 