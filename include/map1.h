#ifndef MAP1_H
#define MAP1_H

#include "raylib.h"
#include "maps.h"

#define MAP1_PLATFORM_COUNT 5

extern Rectangle ground1;
extern Rectangle platforms1[MAP1_PLATFORM_COUNT];

void InitMap1();
void DrawMap1();
void UnloadMap1();

Rectangle* GetMap1Platforms(void);
Rectangle* GetMap1Platforms(void);
Rectangle GetMap1Ground(void);

int GetMap1PlatformCount(void);

#endif
