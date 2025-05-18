#ifndef MAP1_H
#define MAP1_H

#include "raylib.h"

#define MAP1_PLATFORM_COUNT 4  

extern Rectangle ground1;
extern Rectangle platforms1[MAP1_PLATFORM_COUNT];
extern Rectangle ground1_secondFloor_left;
extern Rectangle ground1_secondFloor_right;
extern Rectangle ceiling_left; 
extern Rectangle ceiling_right; 

void InitMap1();
void DrawMap1();
void UnloadMap1();

#endif
