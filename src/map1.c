#include "map1.h"
#include "raylib.h"

#define MAP1_PLATFORM_COUNT 4

Rectangle ground1;
Rectangle ground1_secondFloor_left;
Rectangle ground1_secondFloor_right;
Rectangle platforms1[MAP1_PLATFORM_COUNT];
Rectangle ceiling_left;  
Rectangle ceiling_right; 

static Texture2D background1;
static Texture2D groundTile1;
static Texture2D wallTile;
static Texture2D secondFloorTile;  
static Texture2D ceilingTile_left;
static Texture2D ceilingTile_right;

void DrawWall(int x, int baseY, int heightInPixels) {
    int wallTileHeight = wallTile.height;
    int blocks = heightInPixels / wallTileHeight;

    for (int i = 0; i < blocks; i++) {
        DrawTexture(wallTile, x, baseY - (i + 1) * wallTileHeight, WHITE);
    }

    int remainder = heightInPixels % wallTileHeight;
    if (remainder > 0) {
        Rectangle src = {0, wallTileHeight - remainder, wallTile.width, remainder};
        Rectangle dest = {x, baseY - blocks * wallTileHeight - remainder, wallTile.width, remainder};
        DrawTexturePro(wallTile, src, dest, (Vector2){0, 0}, 0, WHITE);
    }
}

void InitMap1() {
    ground1 = (Rectangle){0, 550, 2000, 500}; 

    float floorY = ground1.y - 200;
    ground1_secondFloor_left = (Rectangle){0, floorY, 1600, 20};  
    ground1_secondFloor_right = (Rectangle){1800, floorY, 300, 20}; 

    background1 = LoadTexture("assets/backgroundMap/backgroundCastle2.png");
    groundTile1 = LoadTexture("assets/sprites/map/ground/groundCastle.png");
    wallTile = LoadTexture("assets/sprites/map/wall/wallCastle.png");

    secondFloorTile = groundTile1;

    ceilingTile_left = groundTile1;  
    ceilingTile_right = groundTile1; 

    ceiling_left = (Rectangle){0, 0, 100, 20};  
    ceiling_right = (Rectangle){300, 0, 2000, 20}; 

    platforms1[0] = ground1_secondFloor_left;
    platforms1[1] = ground1_secondFloor_right;

    platforms1[2] = (Rectangle){400, 450, 120, 20}; 
    platforms1[3] = (Rectangle){700, 380, 120, 20};
}

void DrawMap1() {

    DrawTexture(background1, 0, -490, WHITE); 

    int tileWidth = groundTile1.width;
    int tiles = ground1.width / tileWidth;

    for (int i = 0; i <= tiles; i++) {
        DrawTexture(groundTile1, ground1.x + i * tileWidth, ground1.y, WHITE);
    }

    int tilesLeft = ground1_secondFloor_left.width / tileWidth;
    for (int i = 0; i <= tilesLeft; i++) {
        DrawTexture(secondFloorTile, ground1_secondFloor_left.x + i * tileWidth, ground1_secondFloor_left.y, WHITE);
    }

    int tilesRight = ground1_secondFloor_right.width / tileWidth;
    for (int i = 0; i <= tilesRight; i++) {
        DrawTexture(secondFloorTile, ground1_secondFloor_right.x + i * tileWidth, ground1_secondFloor_right.y, WHITE);
    }

    int tilesCeilingLeft = ceiling_left.width / tileWidth;
    for (int i = 0; i <= tilesCeilingLeft; i++) {
        DrawTexture(ceilingTile_left, ceiling_left.x + i * tileWidth, ceiling_left.y, WHITE);
    }

    int tilesCeilingRight = ceiling_right.width / tileWidth;
    for (int i = 0; i <= tilesCeilingRight; i++) {
        DrawTexture(ceilingTile_right, ceiling_right.x + i * tileWidth, ceiling_right.y, WHITE);
    }

    for (int i = 2; i < MAP1_PLATFORM_COUNT; i++) {
        DrawRectangleRec(platforms1[i], GRAY);
    }

    DrawWall(ground1.x + 100, ground1.y, 185); 
}

void UnloadMap1() {
    UnloadTexture(background1);
    UnloadTexture(groundTile1);
    UnloadTexture(wallTile);
    UnloadTexture(ceilingTile_left);
    UnloadTexture(ceilingTile_right);
}
