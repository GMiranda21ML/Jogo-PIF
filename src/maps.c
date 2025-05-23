#include "maps.h"
#include "raylib.h"
#include "map1.h"
#include "player.h"

typedef struct Spike {
    Rectangle hitbox;
    Texture2D texture;
} Spike;
    
static Rectangle walls[MAX_WALLS];
static int wallCount = 0;

static Rectangle Invisibles[MAX_INVISIBLE];
static int InvisibleCount = 0;

static Rectangle floors[MAX_FLOORS];
static int floorCount = 0;

static Spike spikes[MAX_SPIKES];
static int spikeCount = 0;

static Texture2D spikeTexture;
static Texture2D wallTile;
static Texture2D InvisibleTile;
static Texture2D floorTile;




void LoadSpikeTexture() {
    spikeTexture = LoadTexture("assets/objetos/espinho.png");
}

void CreateSpike(int xInicio, int yInicio, int xFim, int yFim) {
    if (spikeTexture.id == 0) return; // Garante que a textura foi carregada

    int dx = xFim - xInicio;
    int dy = yFim - yInicio;

    // Apenas linhas horizontais ou verticais são suportadas
    if (dx != 0 && dy != 0) return;

    int spikeWidth = spikeTexture.width;
    int spikeHeight = spikeTexture.height;

    if (dx != 0) { // Horizontal
        int step = (dx > 0) ? spikeWidth : -spikeWidth;
        for (int x = xInicio; (step > 0) ? (x <= xFim) : (x >= xFim); x += step) {
            if (spikeCount >= MAX_SPIKES) break;
            spikes[spikeCount++] = (Spike){
                .hitbox = { x, yInicio, spikeWidth, spikeHeight },
                .texture = spikeTexture
            };
        }
    } else if (dy != 0) { // Vertical
        int step = (dy > 0) ? spikeHeight : -spikeHeight;
        for (int y = yInicio; (step > 0) ? (y <= yFim) : (y >= yFim); y += step) {
            if (spikeCount >= MAX_SPIKES) break;
            spikes[spikeCount++] = (Spike){
                .hitbox = { xInicio, y, spikeWidth, spikeHeight },
                .texture = spikeTexture
            };
        }
    } else {
        // Se for apenas um ponto, cria um espinho único
        if (spikeCount < MAX_SPIKES) {
            spikes[spikeCount++] = (Spike){
                .hitbox = { xInicio, yInicio, spikeWidth, spikeHeight },
                .texture = spikeTexture
            };
        }
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

void DrawInvisible(int x, int baseY, int heightInPixels) { //vai ser removido posteriormente
    int InvisibleTileHeight = InvisibleTile.height;
    int blocks = heightInPixels / InvisibleTileHeight;  
    
    for (int i = 0; i < blocks; i++) {
        DrawTexture(InvisibleTile, x, baseY - (i + 1) * InvisibleTileHeight, WHITE);
    }

    int remainder = heightInPixels % InvisibleTileHeight;
    if (remainder > 0) {
        Rectangle srcInv = {0, InvisibleTileHeight - remainder, InvisibleTile.width, remainder};
        Rectangle destInv = {x, baseY - blocks * InvisibleTileHeight - remainder, InvisibleTile.width, remainder};
        DrawTexturePro(InvisibleTile, srcInv, destInv, (Vector2){0, 0}, 0, WHITE);
    }
}

void CreateInvisibleComColisao(int x, int baseY, int altura) {
    if (InvisibleTile.id != 0) {
    DrawInvisible(x, baseY, altura);
    }

    if (InvisibleCount < MAX_INVISIBLE) {
        Rectangle InvisibleRec = {
            x,
            baseY - altura,
            InvisibleTile.width,
            altura
        };
        Invisibles[InvisibleCount++] = InvisibleRec;
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

void DrawFloor(int xInicio, int xFim, int topY, int altura) {
    int tileWidth = floorTile.width;
    int tileHeight = floorTile.height;

    int blocosHorizontais = (xFim - xInicio) / tileWidth;
    int blocosVerticais = altura / tileHeight;

    for (int i = 0; i < blocosVerticais; i++) {
        for (int j = 0; j < blocosHorizontais; j++) {
            DrawTexture(floorTile, xInicio + j * tileWidth, topY + i * tileHeight, WHITE);
        }
    }

    int restoAltura = altura % tileHeight;
    if (restoAltura > 0) {
        for (int j = 0; j < blocosHorizontais; j++) {
            Rectangle src = {0, 0, tileWidth, restoAltura};
            Rectangle dest = {xInicio + j * tileWidth, topY + blocosVerticais * tileHeight, tileWidth, restoAltura};
            DrawTexturePro(floorTile, src, dest, (Vector2){0, 0}, 0, WHITE);
        }
    }

    int restoLargura = (xFim - xInicio) % tileWidth;
    if (restoLargura > 0) {
        for (int i = 0; i < blocosVerticais; i++) {
            Rectangle src = {0, 0, restoLargura, tileHeight};
            Rectangle dest = {xFim - restoLargura, topY + i * tileHeight, restoLargura, tileHeight};
            DrawTexturePro(floorTile, src, dest, (Vector2){0, 0}, 0, WHITE);
        }
    }

    if (restoAltura > 0 && restoLargura > 0) {
        Rectangle src = {0, 0, restoLargura, restoAltura};
        Rectangle dest = {xFim - restoLargura, topY + blocosVerticais * tileHeight, restoLargura, restoAltura};
        DrawTexturePro(floorTile, src, dest, (Vector2){0, 0}, 0, WHITE);
    }
}


void CreateFloorComColisao(int xInicio, int xFim, int topY, int altura) {
    DrawFloor(xInicio, xFim, topY, altura);

    if (floorCount < MAX_FLOORS) {
        Rectangle floorRec = {
            xInicio,
            topY,
            xFim - xInicio,
            altura
        };
        floors[floorCount++] = floorRec;
    }
}


void SetWallTile(Texture2D texture) {
    wallTile = texture;
}

void SetInvisibleTile(Texture2D texture){
    InvisibleTile = texture;
}

void SetFloorTile(Texture2D texture) {
    floorTile = texture;
}

Rectangle *GetInvisible(void){
    return Invisibles;
}
Rectangle *GetWalls(void) {
    return walls;
}

int GetInvisibleCount(void){
    return InvisibleCount;
}

int GetWallCount(void) {
    return wallCount;
}

void UnloadSpikeTexture() {
    UnloadTexture(spikeTexture);
}

void UnloadInvisibleTile(void){
    UnloadTexture(InvisibleTile);
}

void UnloadWallTile(void) {
    UnloadTexture(wallTile);
}

void UnloadFloorTile(void) {
    UnloadTexture(floorTile);
}

Rectangle *GetFloors(void) {
    return floors;
}

int GetFloorCount(void) {
    return floorCount;
}

void ClearInvisibleCollision(){
    InvisibleCount = 0;
}

void ClearWallCollision() {
    wallCount = 0;
}

void ClearFloorCollision() {
    floorCount = 0;
}

void ClearSpikeCollision() {
    spikeCount = 0;
}

void ClearAllMapCollisions() {
    ClearWallCollision();
    ClearFloorCollision();
    ClearSpikeCollision();
    ClearInvisibleCollision();
}