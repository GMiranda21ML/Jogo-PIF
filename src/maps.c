#include "maps.h"
#include "raylib.h"
#include "map1.h"

#define MAX_WALLS 1000

static Rectangle walls[MAX_WALLS];
static int wallCount = 0;

static Texture2D wallTile;

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

void SetWallTile(Texture2D texture) {
    wallTile = texture;
}

Rectangle *GetWalls(void) {
    return walls;
}

int GetWallCount(void) {
    return wallCount;
}

void UnloadWallTile(void) {
    UnloadTexture(wallTile);
}

