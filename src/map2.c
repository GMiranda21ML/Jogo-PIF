#include "map2.h"
#include "maps.h"
#include "raylib.h"
#include "chave.h" // Adicionado

Rectangle ground2;
Rectangle platforms2[MAP2_PLATFORM_COUNT];

static Texture2D background2;
static Texture2D groundTile2;

static Chave chaveMap2; // Adicionado

void InitMap2() {
    ground2 = (Rectangle){0, 550, 2000, 500};

    background2 = LoadTexture("assets/backgroundMap/backgroundPorao.png");
    groundTile2 = LoadTexture("assets/sprites/map/ground/groundPorao.png");
    Texture2D wallTile = LoadTexture("assets/sprites/map/wall/wallPorao.png");
    Texture2D InvisibleTile = LoadTexture("assets/sprites/map/wall/wallCastle.png");
    Texture2D ceilingTile = LoadTexture("assets/sprites/map/ground/groundPorao.png");

    SetWallTile(wallTile);
    SetInvisibleTile(InvisibleTile);
    SetCeilingTile(ceilingTile);

    // Inicializa a chave
    InitChave(&chaveMap2, (Vector2){1500, 360}, "assets/objetos/chave.png");
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
    CreateCeilingComColisao(650,700,465,10);

    CreateCeilingComColisao(1050,1100,400,10);
    CreateCeilingComColisao(1330,1530,450,10);
    CreateCeilingComColisao(1630,1830,400,10);
    CreateCeilingComColisao(1430,1630,300,10);
    CreateCeilingComColisao(1730,1830,200,10);
    CreateCeilingComColisao(1900,2000,100,10);
    CreateCeilingComColisao(1183,1350,200,10);
    CreateCeilingComColisao(1400,1450,100,10);
    CreateCeilingComColisao(1530,1680,100,10);

    CreateCeilingComColisao(1150,1530,0,10);
    CreateCeilingComColisao(1680,2000,0,10);

    // Desenha a chave
    DrawChave(&chaveMap2);
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

// Atualiza a lógica da chave (chamar no loop principal)
void UpdateMap2Chave(Rectangle playerRect, bool *possuiKey) {
    UpdateChave(&chaveMap2, playerRect, possuiKey);
}

void UnloadMap2() {
    UnloadTexture(background2);
    UnloadTexture(groundTile2);
    UnloadSpikeTexture();
    UnloadWallTile();
    UnloadInvisibleTile();

    // Libera textura da chave
    UnloadChave(&chaveMap2);
}
