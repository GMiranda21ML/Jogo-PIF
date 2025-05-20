#include "maps.h"
#include "raylib.h"
#include "map1.h"
#include "player.h"

#define MAX_WALLS 1000
#define MAX_CEILINGS 1000
#define MAX_SPIKES 100

typedef struct Spike {
    Rectangle hitbox;
    Texture2D texture;
} Spike;
    
static Rectangle walls[MAX_WALLS];
static int wallCount = 0;

static Rectangle ceilings[MAX_CEILINGS];
static int ceilingCount = 0;

static Spike spikes[MAX_SPIKES];
static int spikeCount = 0;

static Texture2D spikeTexture;
static Texture2D wallTile;
static Texture2D ceilingTile;




void LoadSpikeTexture() {
    spikeTexture = LoadTexture("assets/objetos/espinho.png");
}

void CreateSpike(int x, int y) {
    if (spikeCount < MAX_SPIKES) {
        Spike spike = {
            .hitbox = { x, y, spikeTexture.width, spikeTexture.height },
            .texture = spikeTexture
        };
        spikes[spikeCount++] = spike;
    }
}

void DrawSpikes() {
    for (int i = 0; i < spikeCount; i++) {
        DrawTexture(spikes[i].texture, spikes[i].hitbox.x, spikes[i].hitbox.y, WHITE);
    }
}

void CheckSpikeDamage(Rectangle playerRect, int *playerHealth) {
    for (int i = 0; i < spikeCount; i++) {
        if (CheckCollisionRecs(playerRect, spikes[i].hitbox)) {
            *playerHealth -= 10; // Aplica 1 de dano, ou o que quiser
        }
    }
}


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

void DrawCeiling(int xInicio, int xFim, int topY, int altura) {
    int tileWidth = ceilingTile.width;
    int tileHeight = ceilingTile.height;

    int blocosHorizontais = (xFim - xInicio) / tileWidth;
    int blocosVerticais = altura / tileHeight;

    for (int i = 0; i < blocosVerticais; i++) {
        for (int j = 0; j < blocosHorizontais; j++) {
            DrawTexture(ceilingTile, xInicio + j * tileWidth, topY + i * tileHeight, WHITE);
        }
    }

    int restoAltura = altura % tileHeight;
    if (restoAltura > 0) {
        for (int j = 0; j < blocosHorizontais; j++) {
            Rectangle src = {0, 0, tileWidth, restoAltura};
            Rectangle dest = {xInicio + j * tileWidth, topY + blocosVerticais * tileHeight, tileWidth, restoAltura};
            DrawTexturePro(ceilingTile, src, dest, (Vector2){0, 0}, 0, WHITE);
        }
    }

    int restoLargura = (xFim - xInicio) % tileWidth;
    if (restoLargura > 0) {
        for (int i = 0; i < blocosVerticais; i++) {
            Rectangle src = {0, 0, restoLargura, tileHeight};
            Rectangle dest = {xFim - restoLargura, topY + i * tileHeight, restoLargura, tileHeight};
            DrawTexturePro(ceilingTile, src, dest, (Vector2){0, 0}, 0, WHITE);
        }
    }

    if (restoAltura > 0 && restoLargura > 0) {
        Rectangle src = {0, 0, restoLargura, restoAltura};
        Rectangle dest = {xFim - restoLargura, topY + blocosVerticais * tileHeight, restoLargura, restoAltura};
        DrawTexturePro(ceilingTile, src, dest, (Vector2){0, 0}, 0, WHITE);
    }
}


void CreateCeilingComColisao(int xInicio, int xFim, int topY, int altura) {
    DrawCeiling(xInicio, xFim, topY, altura);

    if (ceilingCount < MAX_CEILINGS) {
        Rectangle ceilingRec = {
            xInicio,
            topY,
            xFim - xInicio,
            altura
        };
        ceilings[ceilingCount++] = ceilingRec;
    }
}


void SetWallTile(Texture2D texture) {
    wallTile = texture;
}

void SetCeilingTile(Texture2D texture) {
    ceilingTile = texture;
}

Rectangle *GetWalls(void) {
    return walls;
}

int GetWallCount(void) {
    return wallCount;
}

void UnloadSpikeTexture() {
    UnloadTexture(spikeTexture);
}

void UnloadWallTile(void) {
    UnloadTexture(wallTile);
}

void UnloadCeilingTile(void) {
    UnloadTexture(ceilingTile);
}

Rectangle *GetCeilings(void) {
    return ceilings;
}

int GetCeilingCount(void) {
    return ceilingCount;
}

void ClearWallCollision() {
    wallCount = 0;
}

void ClearCeilingCollision() {
    ceilingCount = 0;
}

void ClearSpikeCollision() {
    spikeCount = 0;
}

void ClearAllMapCollisions() {
    ClearWallCollision();
    ClearCeilingCollision();
    ClearSpikeCollision();
}