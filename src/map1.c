#include "map1.h"
#include "raylib.h"

Rectangle ground1;
Rectangle ground1_secondFloor;
Rectangle platforms1[MAP1_PLATFORM_COUNT];

static Texture2D background1;
static Texture2D groundTile1;

void InitMap1() {
    ground1 = (Rectangle){0, 550, 2000, 500}; 
    
    ground1_secondFloor = (Rectangle){0, ground1.y - 200, ground1.width, 20}; 

    background1 = LoadTexture("assets/backgroundMap/backgroundCastle2.png");
    groundTile1 = LoadTexture("assets/sprites/map/ground/groundCastle.png");
}


void DrawMap1() {
    DrawTexture(background1, 0, -490, WHITE); 

    int tileWidth = groundTile1.width;
    int tiles = ground1.width / tileWidth;

    for (int i = 0; i < tiles + 1; i++) {
        DrawTexture(groundTile1, ground1.x + i * tileWidth, ground1.y, WHITE);
    }

    for (int i = 0; i < tiles + 1; i++) {
        DrawTexture(groundTile1, ground1_secondFloor.x + i * tileWidth, ground1_secondFloor.y, WHITE);
    }

    for (int i = 0; i < MAP1_PLATFORM_COUNT; i++) {
        DrawRectangleRec(platforms1[i], GRAY);
    }

}


void UnloadMap1() {
    UnloadTexture(background1);
    UnloadTexture(groundTile1);
}
