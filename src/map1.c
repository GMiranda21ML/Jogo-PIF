#include "map1.h"
#include "maps.h"
#include "raylib.h"

Rectangle ground1;
Rectangle platforms1[MAP1_PLATFORM_COUNT];

static Texture2D background1;
static Texture2D groundTile1;

void InitMap1() {
    ground1 = (Rectangle){1800, 550, 2000, 10};

    background1 = LoadTexture("assets/backgroundMap/backgroundCastle2.png");
    groundTile1 = LoadTexture("assets/sprites/map/ground/groundCastle.png");
    Texture2D wallTile = LoadTexture("assets/sprites/map/wall/wallCastle.png");
    Texture2D floorTile = LoadTexture("assets/sprites/map/ground/groundCastle.png");;
    LoadSpikeTexture();

    SetWallTile(wallTile);
    SetFloorTile(floorTile);

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

    DrawSpikes();
    CreateSpike(830, 260, 1473, 260);
    CreateSpike(640, 260, 830, 260);

    CreateInvisibleComColisao(850, 550, 244);
    CreateInvisibleComColisao(610, 300, 100);
    CreateInvisibleComColisao(620, 200, 100);

    CreateInvisibleComColisao(920, 125, 125);
    CreateInvisibleComColisao(1100, 125, 125);

    CreateInvisibleComColisao(1220, 125, 125);
    CreateInvisibleComColisao(1400, 125, 125);

    CreateInvisibleComColisao(830, 100, 100);
    CreateInvisibleComColisao(400, 100, 100);
    CreateInvisibleComColisao(1680, 550, 550);
    CreateInvisibleComColisao(1785, 390, 390);
    CreateInvisibleComColisao(1475, 100, 100);

    CreateInvisibleComColisao(870, 450, 150);
    CreateInvisibleComColisao(1100, 450, 150);
    CreateInvisibleComColisao(1120, 550, 250);
    CreateInvisibleComColisao(150, 200, 100);
    CreateInvisibleComColisao(120, 100, 100);

    CreateInvisibleComColisao(1500, 550, 244);
    CreateInvisibleComColisao(1505, 300, 200);
    CreateInvisibleComColisao(0, 294, 500);
    CreateInvisibleComColisao(1967, 550, 550);
    CreateInvisibleComColisao(800, 300, 200);
    CreateInvisibleComColisao(250, 200, 200);
    
    CreateWallComColisao(820, 700, 150);
    CreateWallComColisao(1150, 700, 150);

    CreateWallComColisao(1500, 550, 244);
    CreateWallComColisao(1505, 300, 200);
    CreateWallComColisao(0, 294, 500);
    CreateWallComColisao(1967, 550, 550);
    CreateWallComColisao(800, 300, 200);
    CreateWallComColisao(250, 200, 200);

    CreateFloorComColisao(250,620,200,10);
    CreateFloorComColisao(430,830,100,10);
    CreateFloorComColisao(250,2000,0,10);

    CreateFloorComColisao(900,1100,450,10);
    CreateFloorComColisao(0,1530,294,10);
    CreateFloorComColisao(0,850,550,10);
    CreateFloorComColisao(1150,1530,550,10);
    CreateFloorComColisao(1680,1800,550,10);
    CreateFloorComColisao(1817,1967,198,10);
    CreateFloorComColisao(1505,1680,100,10);
    CreateFloorComColisao(1530,1680,295,10);
    CreateFloorComColisao(1530,1680,450,10);
    CreateFloorComColisao(1817,1967,370,10);

    CreateFloorComColisao(950,1105,125,10);
    CreateFloorComColisao(1245,1400,125,10);

    CreateFloorComColisao(0,150,200,10);
    CreateFloorComColisao(0,150,0,10);
    CreateFloorComColisao(150,250,100,10);
    
}

Rectangle* GetMap1Platforms() {
    return platforms1;
}

Rectangle GetMap1Ground(void) {
    return ground1;
}

int GetMap1PlatformCount(void) {
    return MAP1_PLATFORM_COUNT;
}

void UnloadMap1() {
    UnloadTexture(background1);
    UnloadTexture(groundTile1);
    UnloadSpikeTexture();
    UnloadWallTile();
    UnloadFloorTile();
    UnloadInvisibleTile();
}
