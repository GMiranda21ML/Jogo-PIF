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
    Texture2D InvisibleTile = LoadTexture("assets/sprites/map/wall/wallPorao.png");
    Texture2D ceilingTile = LoadTexture("assets/sprites/map/ground/groundCastle.png");;
    LoadSpikeTexture();

    SetWallTile(wallTile); // novo: informa o maps.c sobre a textura da parede
    SetInvisibleTile(InvisibleTile);
    SetCeilingTile(ceilingTile);

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

    CreateInvisibleComColisao(300, 550, 244);

    CreateWallComColisao(1500, 550, 244);
    CreateWallComColisao(1505, 300, 200);
    CreateWallComColisao(0, 294, 500);
    CreateWallComColisao(1967, 550, 550);
    CreateWallComColisao(800, 300, 200);
    CreateWallComColisao(250, 200, 200);

    CreateCeilingComColisao(250,620,200,10);
    CreateCeilingComColisao(430,830,100,10);
    CreateCeilingComColisao(250,2000,0,10);

    CreateCeilingComColisao(900,1100,450,10);
    CreateCeilingComColisao(0,1530,294,10);
    CreateCeilingComColisao(0,850,550,10);
    CreateCeilingComColisao(1150,1530,550,10);
    CreateCeilingComColisao(1680,1800,550,10);
    CreateCeilingComColisao(1817,1967,198,10);
    CreateCeilingComColisao(1505,1680,100,10);
    CreateCeilingComColisao(1530,1680,295,10);
    CreateCeilingComColisao(1530,1680,450,10);
    CreateCeilingComColisao(1817,1967,370,10);

    CreateCeilingComColisao(950,1100,120,10);
    CreateCeilingComColisao(1250,1400,120,10);

    CreateCeilingComColisao(0,150,200,10);
    CreateCeilingComColisao(0,150,0,10);
    CreateCeilingComColisao(150,250,100,10);
    
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
    UnloadCeilingTile();
    UnloadInvisibleTile();
}
