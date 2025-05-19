#include "raylib.h"
#include "sprites.h"
#include "camera.h"
#include "enemy.h"
#include "screens.h"
#include "map1.h"    
#include "levelUp.h"
#include "maps.h"
#include "player.h"
#include <math.h>

#define PLATFORM_COUNT 4
#define ORIGINAL_MAP_ROWS 4
#define ORIGINAL_MAP_COLS 1

int main() {
    InitWindow(800, 600, "Metroid Souls");
    InitAudioDevice();
    SetTargetFPS(60);

    GameScreen currentScreen = RunCutscene();

    Sound hitSound = LoadSound("assets/sound/damageSound/hit.mp3"); 
    Sound levelUpSound = LoadSound("assets/sound/levelUp/levelUpSound.mp3");
    SetSoundVolume(levelUpSound, 1.5f); 
    Music menuMusic = LoadMusicStream("assets/sound/menuSound/menuMusica.mp3");
    Music gameMusic = LoadMusicStream("assets/sound/gameMusic/gameMusicTheme.mp3");
    Sound hitPlayerSound[6] = {
        LoadSound("assets/sound/damageSound/damagePlayer/damagePlayer1.wav"),
        LoadSound("assets/sound/damageSound/damagePlayer/damagePlayer2.wav"),
        LoadSound("assets/sound/damageSound/damagePlayer/damagePlayer3.wav"),
        LoadSound("assets/sound/damageSound/damagePlayer/damagePlayer4.wav"),
        LoadSound("assets/sound/damageSound/damagePlayer/damagePlayer5.wav"),
        LoadSound("assets/sound/damageSound/damagePlayer/damagePlayer6.wav")
    }; 

    for (int i = 0; i < 6; i++) {
        SetSoundVolume(hitPlayerSound[i], 0.5f);
    } 

    Rectangle originalMapMatrix[ORIGINAL_MAP_ROWS][ORIGINAL_MAP_COLS] = {
        {{200, 450, 150, 20}},
        {{450, 350, 150, 20}},
        {{300, 250, 100, 20}},
        {{685, 250, 100, 20}}
    };

    while (currentScreen != SCREEN_EXIT && !WindowShouldClose()) {

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

        if (currentScreen == SCREEN_GAME) {
            gameMusic = LoadMusicStream("assets/sound/gameMusic/gameMusicTheme.mp3");
            PlayMusicStream(gameMusic);

            // Inicializa o player, usando o módulo player.c/player.h
            Player player;
            InitPlayer(&player);

            GroundGrassSprites groundSprites = LoadGroundSprites("assets/sprites/map/ground.json");
            Rectangle ground = { 0, 550, 2000, 500 };
            Rectangle platforms[10];
            int platformcount = PLATFORM_COUNT;

            for (int i = 0; i < PLATFORM_COUNT; i++) {
                platforms[i] = originalMapMatrix[i][0];
            }

            InitMap1();
            MapType currentMap = MAP_ORIGINAL;

            Enemy skeleton;
            InitEnemy(&skeleton, (Vector2){600, 500});
            skeleton.sprites = LoadEnemySprites("assets/sprites/enemy/skeleton_green/skeleton_green.json");

            Camera2D camera = InitCamera(player.position, (Vector2){400, 300});
            Texture2D background = LoadTexture("assets/backgroundMap/backgroundForest.png");

            while (!WindowShouldClose()) {
                UpdateMusicStream(gameMusic);
                float dt = GetFrameTime();

                if (IsKeyPressed(KEY_R)) {
                    menuMusic = LoadMusicStream("assets/sound/menuSound/menuMusica.mp3");
                    currentScreen = SCREEN_MENU;
                    break;
                }

                UpdatePlayer(&player, dt, platforms, platformcount, ground, &skeleton, hitSound, levelUpSound, &currentMap, hitPlayerSound);

                Rectangle playerRect = {player.position.x, player.position.y, player.position.x, player.position.y};
                UpdateEnemy(&skeleton, player.position, dt, skeleton.sprites, playerRect);


                UpdateCameraToFollowPlayer(&camera, player.position, 800, 600, ground.width, ground.y + ground.height);

                BeginDrawing();
                ClearBackground(BLACK);
                BeginMode2D(camera);

                DrawTexture(background, 0, -490, WHITE);

                if (currentMap == MAP_ORIGINAL) {
                    int tileWidth = groundSprites.frames[0].width;
                    int tiles = ground.width / tileWidth;
                    for (int i = 0; i < tiles + 1; i++) {
                        DrawTexture(groundSprites.frames[0], ground.x + i * tileWidth, ground.y, WHITE);
                    }
                    for (int i = 0; i < PLATFORM_COUNT; i++) {
                        DrawRectangleRec(platforms[i], GRAY);
                    }
                } static bool map1Loaded = false;
                if (currentMap == MAP_1 && !map1Loaded) {
                    InitMap1();
                    Rectangle* map1Plats = GetMap1Platforms();
                    platformcount = GetMap1PlatformCount();
                    for (int i = 0; i < platformcount; i++) {
                        platforms[i] = map1Plats[i];
                    }
                    ground = GetMap1Ground();
                    map1Loaded = true;
                } else if (currentMap == MAP_1) {
                    DrawMap1();
                }

                DrawPlayer(&player);

                if (skeleton.alive) {
                    DrawEnemy(&skeleton, GetEnemyTexture(&skeleton, skeleton.sprites));
                }

                EndMode2D();

                // Barra de vida
                DrawRectangle(20, 20, 300, 20, DARKGRAY);
                DrawRectangle(20, 20, 2 * player.playerHealth, 20, RED);
                DrawRectangleLines(20, 20, 300, 20, WHITE);

                EndDrawing();

                if (player.playerHealth <= 0) {
                    currentScreen = SCREEN_GAMEOVER;
                    break;
                }
            }

            FreePlayer(&player);
            UnloadEnemySprites(skeleton.sprites);
            UnloadGroundSprites(groundSprites);
            UnloadTexture(background);
            StopMusicStream(gameMusic);
            UnloadMusicStream(gameMusic);
        }

        if (currentScreen == SCREEN_GAMEOVER) {
            menuMusic = LoadMusicStream("assets/sound/menuSound/menuMusica.mp3");
            GameScreen next = RunGameOver();
            if (next == SCREEN_MENU) {
                currentScreen = SCREEN_MENU;
            } else {
                break;
            }
        }
    }

    for (int i = 0; i < 6; i++) {
        UnloadSound(hitPlayerSound[i]);
    }
    UnloadSound(levelUpSound);
    UnloadSound(hitSound);
    CloseAudioDevice();
    CloseWindow();

    
    return 0;
}