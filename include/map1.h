#ifndef MAP1_H
#define MAP1_H

#include "raylib.h"

#define MAP1_PLATFORM_COUNT 4

extern Rectangle ground1;
extern Rectangle platforms1[MAP1_PLATFORM_COUNT];

void InitMap1();
void DrawMap1(Texture2D groundTile);

#endif
