#include "map1.h"
#include "maps.h"
#include "raylib.h"

#define MAX_WALLS 1000

Rectangle ground1;
Rectangle platforms1[MAP1_PLATFORM_COUNT];
Rectangle walls[MAX_WALLS];

static Texture2D background1;
static Texture2D groundTile1;
static Texture2D wallTile;


int wallCount = 0;

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

void CreateWallComColisao(int x, int baseY, int altura) {
    DrawWall(x, baseY, altura);

    if (wallCount < MAX_WALLS) {
        Rectangle wallRec = {
            x,
            baseY - altura,
            wallTile.width,
            altura
        };
        walls[wallCount++] = wallRec;
    }
}

void InitMap1() {
    ground1 = (Rectangle){0, 550, 2000, 500};

    platforms1[0] = (Rectangle){200, 450, 150, 20};
    platforms1[1] = (Rectangle){450, 350, 150, 20};
    platforms1[2] = (Rectangle){300, 250, 100, 20};
    platforms1[3] = (Rectangle){685, 250, 100, 20};
    platforms1[4] = (Rectangle){800, 250, 100, 20};

    background1 = LoadTexture("assets/backgroundMap/backgroundCastle2.png");
    groundTile1 = LoadTexture("assets/sprites/map/ground/groundCastle.png");
    wallTile = LoadTexture("assets/sprites/map/wall/wallCastle.png");
}

void DrawMap1() {
    DrawTexture(background1, 0, -490, WHITE); 

    int tileWidth = groundTile1.width;
    int tiles = ground1.width / tileWidth;
    for (int i = 0; i < tiles + 1; i++) {
        DrawTexture(groundTile1, ground1.x + i * tileWidth, ground1.y, WHITE);
    }

    for (int i = 0; i < MAP1_PLATFORM_COUNT; i++) {
        DrawRectangleRec(platforms1[i], GRAY);
    }

    CreateWallComColisao(ground1.x + 2000, ground1.y, 185);

}

void UnloadMap1() {
    UnloadTexture(background1);
    UnloadTexture(groundTile1);
    UnloadTexture(wallTile);
}
