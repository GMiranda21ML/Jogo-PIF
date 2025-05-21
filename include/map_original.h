#ifndef MAP_ORIGINAL_H
#define MAP_ORIGINAL_H

#include "raylib.h"

void InitMapOriginal(void);
Rectangle* GetMapOriginalPlatforms(void);
int GetMapOriginalPlatformCount(void);
Rectangle GetMapOriginalGround(void);
void DrawMapOriginal(void);

#endif // MAP_ORIGINAL_H
