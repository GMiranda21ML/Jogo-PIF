#include "map3.h"
#include "maps.h"
#include "raylib.h"
#include "chave.h"
#include "player.h"
#include "porta.h"


Rectangle ground3;
Rectangle platforms3[MAP3_PLATFORM_COUNT];

static Porta portaDoCastelo;
static Texture2D background3;
static Texture2D groundTile3;

void InitMap3() {
    ground3 = (Rectangle){250, 550, 2000, 10};

    background3 = LoadTexture("assets/backgroundMap/backgroundCastle2.png");
    groundTile3 = LoadTexture("assets/sprites/map/ground/groundCastle.png");
    Texture2D wallTile = LoadTexture("assets/sprites/map/wall/wallCastle.png");
    Texture2D floorTile = LoadTexture("assets/sprites/map/ground/groundCastle.png");
    Texture2D portaFechada = LoadTexture("assets/objetos/closedDoorStone.png");
    Texture2D portaAberta = LoadTexture("assets/objetos/espinho.png");

    InitPorta(&portaDoCastelo, (Vector2){1990, 185}, portaFechada, portaAberta);

    LoadSpikeTexture();
    
    SetWallTile(wallTile);
    SetFloorTile(floorTile);
}

void DrawMap3() {
    DrawTexture(background3, 0, -490, WHITE); 

    int tileWidth = groundTile3.width;
    int tiles = ground3.width / tileWidth;
    for (int i = 0; i < tiles + 1; i++) {
        DrawTexture(groundTile3, ground3.x + i * tileWidth, ground3.y, WHITE);
    }

    for (int i = 0; i < MAP3_PLATFORM_COUNT; i++) {
        DrawRectangleRec(platforms3[i], GRAY);
    }

    DrawPorta(&portaDoCastelo);
    DrawSpikes();
    CreateSpike(350, 215, 1750, 215);



    CreateInvisibleComColisao(150, 550, 200);
    CreateInvisibleComColisao(220, 550, 200);
    CreateInvisibleComColisao(1837, 470, 200);
    CreateInvisibleComColisao(1790, 390, 100);
    CreateInvisibleComColisao(370, 390, 100);
    CreateInvisibleComColisao(320, 250, 200);
    CreateInvisibleComColisao(1740, 250, 200);


    CreateInvisibleComColisao(620, 160, 160);
    CreateInvisibleComColisao(970, 180, 180);
    CreateInvisibleComColisao(1320, 160, 160);
    CreateInvisibleComColisao(1570, 180, 180);


    CreateInvisibleComColisao(550, 180, 180);
    CreateInvisibleComColisao(850, 160, 160);
    CreateInvisibleComColisao(1200, 180, 180);
    CreateInvisibleComColisao(1500, 160, 160);
    CreateInvisibleComColisao(1700, 180, 180);


    CreateInvisibleComColisao(170, 350, 300);
    CreateInvisibleComColisao(100, 350, 300);
    CreateInvisibleComColisao(300, 350, 50);



    CreateWallComColisao(0, 550, 550);
    CreateWallComColisao(1970, 550, 300);
    CreateWallComColisao(1970, 190, 215);
    CreateWallComColisao(2000, 550, 550);

    CreateFloorComColisao(0, 100, 320, 10);
    CreateFloorComColisao(200, 300, 350, 10);
    CreateFloorComColisao(1867, 1967, 470, 10);

    CreateFloorComColisao(200, 2000, 250, 10);
    CreateFloorComColisao(400, 1797, 380, 10);

    CreateFloorComColisao(350, 550, 180, 10);
    CreateFloorComColisao(650, 850, 160, 10);
    CreateFloorComColisao(1000, 1200, 180, 10);
    CreateFloorComColisao(1350, 1500, 160, 10);
    CreateFloorComColisao(1600, 1700, 180, 10);

    CreateFloorComColisao(0, 2000, 0, 10);
    CreateFloorComColisao(0, 150, 550, 10);
}

Rectangle* GetMap3Platforms() {
    return platforms3;
}

Rectangle GetMap3Ground(void) {
    return ground3;
}

int GetMap3PlatformCount(void) {
    return MAP3_PLATFORM_COUNT;
}

void UnloadMap3() {
    UnloadTexture(background3);
    UnloadTexture(groundTile3);
    UnloadPorta(&portaDoCastelo);
    UnloadSpikeTexture();
    UnloadWallTile();
    UnloadInvisibleTile();
}
