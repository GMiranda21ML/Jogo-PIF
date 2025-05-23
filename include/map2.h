#ifndef MAP2_H
#define MAP2_H

#include "raylib.h"
#include "chave.h"

#define MAP2_PLATFORM_COUNT 10

void InitMap2(void);
void DrawMap2(void);
void UnloadMap2(void);

Rectangle* GetMap2Platforms(void);
Rectangle GetMap2Ground(void);
int GetMap2PlatformCount(void);

Chave* GetMap2Chave(void);

#endif

