#include "map2.h"
#include "maps.h"
#include "raylib.h"
#include "chave.h"
#include "player.h"

Rectangle ground2;
Rectangle platforms2[MAP2_PLATFORM_COUNT];

static Chave chaveMap2;

static Texture2D background2;
static Texture2D groundTile2;
static Texture2D chaveTextura;

void InitMap2() {
    ground2 = (Rectangle){0, 550, 2000, 500};

    background2 = LoadTexture("assets/backgroundMap/backgroundPorao.png");
    groundTile2 = LoadTexture("assets/sprites/map/ground/groundPorao.png");
    Texture2D wallTile = LoadTexture("assets/sprites/map/wall/wallPorao.png");
    Texture2D floorTile = LoadTexture("assets/sprites/map/ground/groundPorao.png");
    chaveTextura = LoadTexture("assets/objetos/chave.png");
    InitChave(&chaveMap2, (Vector2){1700, 350}, chaveTextura);

    SetWallTile(wallTile);
    SetFloorTile(floorTile);

}

void DrawMap2() {
    DrawTexture(background2, 0, -490, WHITE); 

    int tileWidth = groundTile2.width;
    int tiles = ground2.width / tileWidth;
    for (int i = 0; i < tiles + 1; i++) {
        DrawTexture(groundTile2, ground2.x + i * tileWidth, ground2.y, WHITE);
    }

    for (int i = 0; i < MAP2_PLATFORM_COUNT; i++) {
        DrawRectangleRec(platforms2[i], GRAY);
    }

    DrawChave(&chaveMap2);
    
    DrawSpikes();
    CreateSpike(0, 520, 2000, 520);
    
    CreateInvisibleComColisao(520, 200, 200);
    CreateInvisibleComColisao(700, 200, 200);
    CreateInvisibleComColisao(1350, 200, 200);
    CreateInvisibleComColisao(1400, 300, 150);
    CreateInvisibleComColisao(1630, 300, 150);
    CreateInvisibleComColisao(1600, 400, 90);
    CreateInvisibleComColisao(1820, 400, 90);
    CreateInvisibleComColisao(1300, 450, 90);
    CreateInvisibleComColisao(1520, 450, 90);
    CreateInvisibleComColisao(1500, 100, 100);
    CreateInvisibleComColisao(1680, 100, 100);
    CreateInvisibleComColisao(1150, 200, 200);

    CreateFloorComColisao(850 , 882,100 ,10);
    
    CreateWallComColisao(1150, 200, 200);
    CreateWallComColisao(850, 100, 100);
    
    CreateFloorComColisao(850 , 1183,200 ,10);
    CreateFloorComColisao(0 , 850,0 ,10);
    
    CreateWallComColisao(0, 550, 550);
    CreateWallComColisao(1968, 550, 550);
    
    CreateFloorComColisao(0,100,430,10);
    CreateFloorComColisao(550,700,200,10);
    CreateFloorComColisao(250,400,200,10);
    
    CreateFloorComColisao(300,350,450,10);
    CreateFloorComColisao(550,600,480,10);
    CreateFloorComColisao(850,900,480,10);
    CreateFloorComColisao(650,700,465,10);
    
    CreateFloorComColisao(1050,1100,400,10);
    CreateFloorComColisao(1330,1530,450,10);
    CreateFloorComColisao(1630,1830,400,10);
    CreateFloorComColisao(1430,1630,300,10);
    CreateFloorComColisao(1730,1830,200,10);
    CreateFloorComColisao(1900,2000,100,10);
    CreateFloorComColisao(1183,1350,200,10);
    CreateFloorComColisao(1400,1450,100,10);
    CreateFloorComColisao(1530,1680,100,10);
    
    CreateFloorComColisao(1150,1530,0,10);
    CreateFloorComColisao(1680,2000,0,10);

}

Rectangle* GetMap2Platforms() {
    return platforms2;
}

Rectangle GetMap2Ground(void) {
    return ground2;
}

int GetMap2PlatformCount(void) {
    return MAP2_PLATFORM_COUNT;
}

Chave* GetMap2Chave(void) {
return &chaveMap2;
}

void UnloadMap2() {
    UnloadTexture(background2);
    UnloadTexture(groundTile2);
    UnloadSpikeTexture();
    UnloadWallTile();
    UnloadInvisibleTile();
    UnloadChave(&chaveMap2);
}
