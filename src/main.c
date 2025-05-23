#include "raylib.h"
#include "sprites.h"
#include "camera.h"
#include "enemy.h"
#include "screens.h"
#include "map1.h"  
#include "map2.h"  
#include "levelUp.h"
#include "map3.h"
#include "maps.h"
#include "player.h"
#include <stdio.h>
#include "map_original.h"
#include <math.h>
#include "chave.h"

#define MAX_ENEMIES 20
Enemy enemies[MAX_ENEMIES];
int enemyCount = 0;

int UnloadEnemysMap(void){
    for (int i = 0; i < enemyCount; i++) {
        UnloadEnemySprites(enemies[i].sprites);
    }
    
    return 0;
}

int main() {
    InitWindow(800, 600, "Metroid Souls");
    InitAudioDevice();
    SetTargetFPS(60);

    bool mensagemSemChave = false;
    float mensagemTimer = 0.0f;
    const float MENSAGEM_DURACAO = 3.0f;

    GameScreen currentScreen = RunCutscene();
    Sound hitSound = LoadSound("assets/sound/damageSound/hit.mp3"); 
    Sound levelUpSound = LoadSound("assets/sound/levelUp/levelUpSound.mp3");
    SetSoundVolume(levelUpSound, 1.5f); 
    Music menuMusic = LoadMusicStream("assets/sound/menuSound/menuMusica.mp3");
    Music originalMapMusic = LoadMusicStream("assets/sound/gameMusic/forestMap/forestMusic.mp3");
    Music map1and3Music = LoadMusicStream("assets/sound/gameMusic/gameMusicTheme.mp3");
    Music caveMusic = LoadMusicStream("assets/sound/gameMusic/caveMusic/caveMusic.mp3");


    Music* currentMapMusic = NULL;
    MapType lastMap = -1;

    Sound hitPlayerSound[4] = {
        LoadSound("assets/sound/damageSound/damagePlayer/damagePlayer2.wav"),
        LoadSound("assets/sound/damageSound/damagePlayer/damagePlayer3.wav"),
        LoadSound("assets/sound/damageSound/damagePlayer/damagePlayer5.wav"),
        LoadSound("assets/sound/damageSound/damagePlayer/damagePlayer6.wav")
    }; 

    for (int i = 0; i < 4; i++) {
        SetSoundVolume(hitPlayerSound[i], 0.3f);
    } 

    static bool mapOriginalLoaded = false;
    static bool map1Loaded = false;
    static bool map2Loaded = false;
    static bool map3Loaded = false;

    while (currentScreen != SCREEN_EXIT && !WindowShouldClose()) {
        float totalGameTime = 0.0f;


        if (currentScreen == SCREEN_GAME || currentScreen == SCREEN_EXIT) {
            StopMusicStream(menuMusic);
            UnloadMusicStream(menuMusic);
        }

        if (currentScreen == SCREEN_MENU) {
            PlayMusicStream(menuMusic);
            currentScreen = RunMenu(menuMusic);
        }

        if (currentScreen == SCREEN_KEYBOARD) {
            currentScreen = RunKeyboardScreen(menuMusic);
        }

        if (currentScreen == SCREEN_SCORE) {
            currentScreen = RunBestTimeScreen(menuMusic);
        }

        if (currentScreen == SCREEN_GAME) {
            Player player;
            InitPlayer(&player);

            GroundGrassSprites groundSprites = LoadGroundSprites("assets/sprites/map/ground.json");
            Rectangle ground = { 0, 550, 2000, 500 };
            Rectangle platforms[10];
            int platformcount = 0;

            InitMap1();
            MapType currentMap = MAP_ORIGINAL;

            Enemy enemies[MAX_ENEMIES];
            int enemyCount = 4;
            InitEnemy(&enemies[0], (Vector2){600, 500}, 1.0f, 10, 5, 70);
            enemies[0].sprites = LoadEnemySprites("assets/sprites/enemy/skeleton_green/skeleton_green.json");

            InitEnemy(&enemies[1], (Vector2){1200, 500}, 1.0f, 10, 5, 70);
            enemies[1].sprites = LoadEnemySprites("assets/sprites/enemy/skeleton_green/skeleton_green.json");

            InitEnemy(&enemies[2], (Vector2){1700, 500}, 1.0f, 10, 5, 70);
            enemies[2].sprites = LoadEnemySprites("assets/sprites/enemy/skeleton_green/skeleton_green.json");

            InitEnemy(&enemies[3], (Vector2){1930, 500}, 1.0f, 10, 5, 70);
            enemies[3].sprites = LoadEnemySprites("assets/sprites/enemy/skeleton_green/skeleton_green.json");

            Camera2D camera = InitCamera(player.position, (Vector2){400, 300});
            Texture2D background = LoadTexture("assets/backgroundMap/backgroundForest.png");

            while (!WindowShouldClose()) {
                float dt = GetFrameTime();

                if (currentScreen == SCREEN_GAME) {
                    totalGameTime += dt;
                }

                 if (mensagemSemChave) {
                    mensagemTimer -= dt;
                    if (mensagemTimer <= 0.0f) {
                        mensagemSemChave = false;
                    }
                }

                if (currentMap != lastMap) {
                    Music* newMapMusic = NULL;
                
                    switch (currentMap) {
                        case MAP_ORIGINAL: 
                            newMapMusic = &originalMapMusic; 
                            break;
                        case MAP_1:
                        case MAP_3:
                            newMapMusic = &map1and3Music;
                            break;
                        case MAP_2: 
                            newMapMusic = &caveMusic; 
                            break;
                        default: 
                            newMapMusic = NULL; 
                            break;
                    }
                
                    if (newMapMusic != currentMapMusic) {
                        if (currentMapMusic != NULL) StopMusicStream(*currentMapMusic);
                        currentMapMusic = newMapMusic;
                        if (currentMapMusic != NULL) PlayMusicStream(*currentMapMusic);
                    }
                
                    lastMap = currentMap;
                }

                if (currentMapMusic != NULL) UpdateMusicStream(*currentMapMusic);

                if (IsKeyPressed(KEY_R)) {
                    ResetarChaveColetada();
                    if (currentMapMusic != NULL) {
                        StopMusicStream(*currentMapMusic);
                        currentMapMusic = NULL;
                    }
                    lastMap = -1;
                    menuMusic = LoadMusicStream("assets/sound/menuSound/menuMusica.mp3");
                    currentScreen = SCREEN_MENU;
                    break;
                }

                Rectangle areaPorta = {1950, 185, 100, 100};
                if (IsKeyPressed(KEY_E) && player.PossuiKey && CheckCollisionPointRec(player.position, areaPorta)) {
                    currentScreen = RunEpilogueCutscene(totalGameTime);
                    if (currentScreen == SCREEN_MENU) {
                        break;
                    }
                }else if (IsKeyPressed(KEY_E) && !player.PossuiKey && CheckCollisionPointRec(player.position, areaPorta)) {
                    mensagemSemChave = true;
                    mensagemTimer = MENSAGEM_DURACAO;
                }
                                
                UpdatePlayer(&player, dt, platforms, platformcount, ground, enemies, enemyCount, hitSound, levelUpSound, &currentMap, hitPlayerSound);

                for (int i = 0; i < enemyCount; i++) {
                    UpdateEnemy(&enemies[i], player.position, dt, enemies[i].sprites);
                }

                UpdateCameraToFollowPlayer(&camera, player.position, 800, 600, ground.width, ground.y + ground.height);

                BeginDrawing();
                ClearBackground(BLACK);
                BeginMode2D(camera);
                DrawTexture(background, 0, -490, WHITE);
                

                if (currentMap == MAP_1 && !map1Loaded) {
                    enemyCount = UnloadEnemysMap();

                    ClearAllMapCollisions();
                    InitMap1();
                    enemyCount = 8;
                    InitEnemy(&enemies[0], (Vector2){200, 500}, 1.0f, 10, 5, 70);
                    enemies[0].sprites = LoadEnemySprites("assets/sprites/enemy/skeleton_green/skeleton_green.json");

                    InitEnemy(&enemies[1], (Vector2){700, 500}, 1.0f, 15, 8, 70);
                    enemies[1].sprites = LoadEnemySprites("assets/sprites/enemy/skeleton_red/skeleton_red.json");

                    InitEnemy(&enemies[2], (Vector2){960, 80}, 1.0f, 15, 8, 70);
                    enemies[2].sprites = LoadEnemySprites("assets/sprites/enemy/skeleton_red/skeleton_red.json");

                    InitEnemy(&enemies[3], (Vector2){1260, 68}, 0.35f, 20, 10, 50);
                    enemies[3].sprites = LoadEnemySprites("assets/sprites/enemy/dhuron/dhuron.json");

                    InitEnemy(&enemies[4], (Vector2){500, 155}, 1.0f, 15, 8, 70);
                    enemies[4].sprites = LoadEnemySprites("assets/sprites/enemy/skeleton_red/skeleton_red.json");

                    InitEnemy(&enemies[5], (Vector2){250, 235}, 1.0f, 30, 15, 60);
                    enemies[5].sprites = LoadEnemySprites("assets/sprites/enemy/skeleton_armor/skeleton_armor.json");

                    InitEnemy(&enemies[6], (Vector2){1540, 250}, 1.0f, 15, 8, 70);
                    enemies[6].sprites = LoadEnemySprites("assets/sprites/enemy/skeleton_red/skeleton_red.json");

                    InitEnemy(&enemies[7], (Vector2){1920, 315}, 0.35f, 20, 10, 50);
                    enemies[7].sprites = LoadEnemySprites("assets/sprites/enemy/dhuron/dhuron.json");

                    Rectangle* map1Plats = GetMap1Platforms();
                    platformcount = GetMap1PlatformCount();

                    for (int i = 0; i < platformcount; i++) {
                        platforms[i] = map1Plats[i];
                    }
                    for (int i = platformcount; i < 10; i++) {
                        platforms[i] = (Rectangle){0,0,0,0};
                    }

                    ground = GetMap1Ground();

                    mapOriginalLoaded = false;
                    map1Loaded = true;
                    map2Loaded = false;
                    map3Loaded = false;

                } else if (currentMap == MAP_2 && !map2Loaded) {
                    enemyCount = UnloadEnemysMap();
                    ClearAllMapCollisions();
                    InitMap2();

                    enemyCount = 4;
                    InitEnemy(&enemies[0], (Vector2){570, 150}, 1.0f, 15, 8, 70);
                    enemies[0].sprites = LoadEnemySprites("assets/sprites/enemy/skeleton_red/skeleton_red.json");

                    InitEnemy(&enemies[1], (Vector2){1460, 400}, 0.35f, 20, 10, 50);
                    enemies[1].sprites = LoadEnemySprites("assets/sprites/enemy/dhuron/dhuron.json");

                    InitEnemy(&enemies[2], (Vector2){1440, 250}, 1.0f, 15, 8, 70);
                    enemies[2].sprites = LoadEnemySprites("assets/sprites/enemy/skeleton_red/skeleton_red.json");

                    InitEnemy(&enemies[3], (Vector2){1770, 350}, 0.35f, 20, 10, 50);
                    enemies[3].sprites = LoadEnemySprites("assets/sprites/enemy/dhuron/dhuron.json");

                    Rectangle* map2Plats = GetMap2Platforms();
                    platformcount = GetMap2PlatformCount();

                    for (int i = 0; i < platformcount; i++) {
                        platforms[i] = map2Plats[i];
                    }
                    for (int i = platformcount; i < 10; i++) {
                        platforms[i] = (Rectangle){0,0,0,0};
                    }

                    ground = GetMap2Ground();
                    
                    mapOriginalLoaded = false;
                    map2Loaded = true;
                    map1Loaded = false;
                    map3Loaded = false;

                }else if (currentMap == MAP_ORIGINAL && !mapOriginalLoaded) {
                    enemyCount = UnloadEnemysMap();
                    ClearAllMapCollisions();
                    InitMapOriginal();

                    enemyCount = 4;
                    InitEnemy(&enemies[0], (Vector2){600, 500}, 1.0f, 10, 5, 70);
                    enemies[0].sprites = LoadEnemySprites("assets/sprites/enemy/skeleton_green/skeleton_green.json");

                    InitEnemy(&enemies[1], (Vector2){1200, 500}, 1.0f, 10, 5, 70);
                    enemies[1].sprites = LoadEnemySprites("assets/sprites/enemy/skeleton_green/skeleton_green.json");
        
                    InitEnemy(&enemies[2], (Vector2){1700, 500}, 1.0f, 10, 5, 70);
                    enemies[2].sprites = LoadEnemySprites("assets/sprites/enemy/skeleton_green/skeleton_green.json");
        
                    InitEnemy(&enemies[3], (Vector2){1930, 500}, 1.0f, 10, 5, 70);
                    enemies[3].sprites = LoadEnemySprites("assets/sprites/enemy/skeleton_green/skeleton_green.json");

                    ground = GetMapOriginalGround();

                    mapOriginalLoaded = true;
                    map1Loaded = map2Loaded = map3Loaded = false;

                    } else if (currentMap == MAP_3 && !map3Loaded) {
                    enemyCount = UnloadEnemysMap();
                    ClearAllMapCollisions();
                    InitMap3();

                    enemyCount = 4;
                    InitEnemy(&enemies[0], (Vector2){500, 320}, 1.0f, 30, 15, 60);
                    enemies[0].sprites = LoadEnemySprites("assets/sprites/enemy/skeleton_armor/skeleton_armor.json");

                    InitEnemy(&enemies[1], (Vector2){1200, 490}, 1.0f, 30, 15, 60);
                    enemies[1].sprites = LoadEnemySprites("assets/sprites/enemy/skeleton_armor/skeleton_armor.json");

                    InitEnemy(&enemies[2], (Vector2){810, 115}, 1.0f, 15, 8, 70);
                    enemies[2].sprites = LoadEnemySprites("assets/sprites/enemy/skeleton_red/skeleton_red.json");

                    InitEnemy(&enemies[3], (Vector2){1450, 108}, 0.35f, 20, 10, 50);
                    enemies[3].sprites = LoadEnemySprites("assets/sprites/enemy/dhuron/dhuron.json");

                    Rectangle* map3Plats = GetMap3Platforms();
                    platformcount = GetMap3PlatformCount();

                    for (int i = 0; i < platformcount; i++) {
                        platforms[i] = map3Plats[i];
                    }
                    for (int i = platformcount; i < 10; i++) {
                        platforms[i] = (Rectangle){0,0,0,0};
                    }

                    ground = GetMap3Ground();

                    mapOriginalLoaded = false;
                    map3Loaded = true;
                    map1Loaded = false;
                    map2Loaded = false;
                }

                if (currentMap == MAP_ORIGINAL) DrawMapOriginal();
                if (currentMap == MAP_1) DrawMap1();
                if (currentMap == MAP_2) DrawMap2();
                if (currentMap == MAP_3) DrawMap3();

                DrawPlayer(&player);
                for (int i = 0; i < enemyCount; i++) {
                    if (enemies[i].alive) {
                        DrawEnemy(&enemies[i], GetEnemyTexture(&enemies[i], enemies[i].sprites));
                    }
                }

                EndMode2D();

                if (player.PossuiKey) {
                    DrawText("Possui chave", GetScreenWidth() - 180, 20, 20, YELLOW);
                }

                DrawRectangle(20, 20, 300, 20, DARKGRAY);
                DrawRectangle(20, 20, 2 * player.playerHealth, 20, RED);
                DrawRectangleLines(20, 20, 300, 20, WHITE);

                if (mensagemSemChave) {
                    DrawText("Você precisa da chave para abrir a porta!", 350, 20, 20, RED);
                }

                EndDrawing();

                if (player.playerHealth <= 0) {
                    if (currentMapMusic != NULL) {
                        StopMusicStream(*currentMapMusic);
                        currentMapMusic = NULL;
                    }
                    lastMap = -1;
                    currentScreen = SCREEN_GAMEOVER;
                    break;
                }
            }

            FreePlayer(&player);
            for (int i = 0; i < enemyCount; i++) {
                UnloadEnemySprites(enemies[i].sprites);
            }
            UnloadGroundSprites(groundSprites);
            UnloadTexture(background);

            if (currentMapMusic != NULL) {
                StopMusicStream(*currentMapMusic);
                currentMapMusic = NULL;
            }
            lastMap = -1;
        }

        if (currentScreen == SCREEN_GAMEOVER) {
            ResetarChaveColetada();
            if (currentMapMusic != NULL) {
                StopMusicStream(*currentMapMusic);
                currentMapMusic = NULL;
            }
            lastMap = -1;

            menuMusic = LoadMusicStream("assets/sound/menuSound/menuMusica.mp3");
            GameScreen next = RunGameOver();
            if (next == SCREEN_MENU) {
                currentScreen = SCREEN_MENU;
            } else {
                break;
            }
        }
    }

    for (int i = 0; i < 4; i++) {
        UnloadSound(hitPlayerSound[i]);
    }
    UnloadSound(levelUpSound);
    UnloadSound(hitSound);
    if (currentMapMusic != NULL) StopMusicStream(*currentMapMusic);
    UnloadMusicStream(originalMapMusic);
    UnloadMusicStream(map1and3Music);
    UnloadMusicStream(caveMusic);

    CloseAudioDevice();
    CloseWindow();

    return 0;
}
