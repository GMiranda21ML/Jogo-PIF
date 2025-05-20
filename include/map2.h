#ifndef MAP2_H
#define MAP2_H

#include "raylib.h"
#include "maps.h"

#define MAP2_PLATFORM_COUNT 5  // Altere conforme o número de plataformas reais

extern Rectangle ground2;
extern Rectangle platforms2[MAP2_PLATFORM_COUNT];

void InitMap2(void);
void DrawMap2(void);
void UnloadMap2(void);

Rectangle* GetMap2Platforms(void);
Rectangle GetMap2Ground(void);
int GetMap2PlatformCount(void);

#endif
