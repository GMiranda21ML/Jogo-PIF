#include "map2.h"
#include "maps.h"
#include "raylib.h"

Rectangle ground2;
Rectangle platforms2[MAP2_PLATFORM_COUNT];

static Texture2D background2;
static Texture2D groundTile2;

void InitMap2() {
    ground2 = (Rectangle){0, 550, 2000, 500};

    background2 = LoadTexture("assets/backgroundMap/backgroundPorao.png");
    groundTile2 = LoadTexture("assets/sprites/map/ground/groundPorao.png");
    Texture2D wallTile = LoadTexture("assets/sprites/map/wall/wallPorao.png");
    Texture2D ceilingTile = LoadTexture("assets/sprites/map/ground/groundPorao.png");

    SetWallTile(wallTile);
    SetCeilingTile(ceilingTile);
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
    
    DrawSpikes();
    CreateSpike(0, 500, 2000, 500);

    CreateCeilingComColisao(850 , 882,100 ,10);


    CreateWallComColisao(1150, 200, 200);
    CreateWallComColisao(850, 100, 100);

    CreateCeilingComColisao(850 , 1183,200 ,10);
    CreateCeilingComColisao(0 , 850,0 ,10);

    CreateWallComColisao(0, 550, 550);
    CreateWallComColisao(1968, 550, 550);

    CreateCeilingComColisao(0,100,430,10);
    CreateCeilingComColisao(550,700,200,10);
    CreateCeilingComColisao(250,400,200,10);

    CreateCeilingComColisao(300,350,450,10);
    CreateCeilingComColisao(550,600,480,10);
    CreateCeilingComColisao(850,900,480,10);
    CreateCeilingComColisao(650,700,480,10);

    CreateCeilingComColisao(1050,1100,400,10);
    CreateCeilingComColisao(1330,1530,450,10);
    CreateCeilingComColisao(1630,1830,400,10);
    CreateCeilingComColisao(1430,1630,300,10);
    CreateCeilingComColisao(1730,1830,200,10);
    CreateCeilingComColisao(1900,2000,100,10);
    CreateCeilingComColisao(1183,1350,200,10);
    CreateCeilingComColisao(1400,1450,150,10);
    CreateCeilingComColisao(1530,1680,100,10);

    CreateCeilingComColisao(1150,1530,0,10);
    CreateCeilingComColisao(1680,2000,0,10);
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

void UnloadMap2() {
    UnloadTexture(background2);
    UnloadTexture(groundTile2);
    UnloadSpikeTexture();
    UnloadWallTile();
}
