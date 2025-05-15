#include "raylib.h"
#include "sprites.h"
#include "camera.h"
#include "enemy.h"
#include "screens.h"
#include "map1.h"    // Novo mapa
#include <math.h>

#define PLATFORM_COUNT 4
#define DETECTION_RADIUS 200.0f
#define SKELETON_HIT_DURATION 0.2f
#define PLAYER_HIT_COOLDOWN 0.5f

typedef enum {
    MAP_ORIGINAL,
    MAP_1
} MapType;

int main() {
    InitWindow(800, 600, "Metroid Souls");
    InitAudioDevice();
    SetTargetFPS(60);

    GameScreen currentScreen = RunCutscene();

    Sound hitSound = LoadSound("assets/sound/damageSound/hit.mp3"); 
    // SetSoundVolume(hitSound, 0.5f); se quiser diminuir o som do hit
    Music menuMusic = LoadMusicStream("assets/sound/menuSound/menuMusica.mp3");
    Music gameMusic = LoadMusicStream("assets/sound/gameMusic/gameMusicTheme.mp3");
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
            int playerHealth = 150;
            float playerHitTimer = 0.0f;

            PlayerSprites sprites = LoadPlayerSprites("assets/sprites/player/player.json");
            GroundGrassSprites groundSprites = LoadGroundSprites("assets/sprites/map/ground.json");

            if (sprites.frame_change <= 0.0f) sprites.frame_change = 0.10f;

            // Map original
            Rectangle ground = { 0, 550, 2000, 500 };
            Rectangle platforms[PLATFORM_COUNT] = {
                {200, 450, 150, 20},
                {450, 350, 150, 20},
                {300, 250, 100, 20},
                {685, 250, 100, 20}
            };

            // Inicializa o mapa 1 externo
            InitMap1();

            MapType currentMap = MAP_ORIGINAL;

            Vector2 position = {400, 500};
            Vector2 velocity = {0, 0};
            float gravity = 900.0f;
            float jumpForce = -450.0f;
            bool isOnGround = false;

            bool attacking = false;
            int attackFacing = 1;

            float speed = 200.0f;
            float timer = 0;
            int frame = 0;
            int facing = 1;
            static Animation previousAnim = {0};

            Enemy skeleton;
            InitEnemy(&skeleton, (Vector2){600, 500});
            skeleton.sprites = LoadEnemySprites("assets/sprites/enemy/skeleton_green/skeleton_green.json");

            Camera2D camera = InitCamera(position, (Vector2){400, 300});

            Texture2D background = LoadTexture("assets/backgroundMap/backgroundForest.png");

            while (!WindowShouldClose()) {
                UpdateMusicStream(gameMusic); 
                float dt = GetFrameTime();
                bool moving = false;

                if (playerHitTimer > 0.0f) playerHitTimer -= dt;

                // Movimento
                if (IsKeyDown(KEY_D)) {
                    position.x += speed * dt;
                    facing = 1;
                    moving = true;
                } else if (IsKeyDown(KEY_A)) {
                    position.x -= speed * dt;
                    facing = -1;
                    moving = true;
                }

                if (IsKeyPressed(KEY_R)) {
                    menuMusic = LoadMusicStream("assets/sound/menuSound/menuMusica.mp3");
                    currentScreen = SCREEN_MENU;
                    break;
                }

                // Limites horizontais no mapa atual
                if (position.x < 0) position.x = 0;

                // Troca de mapa ao chegar na ponta direita do mapa original
                if (currentMap == MAP_ORIGINAL) {
                    if (position.x + sprites.walk_right.frames[0].width >= ground.width) {
                        // Troca para mapa 1
                        currentMap = MAP_1;
                        ground = ground1;
                        for (int i = 0; i < MAP1_PLATFORM_COUNT; i++) {
                            platforms[i] = platforms1[i];
                        }
                        position = (Vector2){0, 500};
                    }
                }

                // Limita a posição no mapa atual
                if (currentMap == MAP_ORIGINAL) {
                    if (position.x + sprites.walk_right.frames[0].width > ground.width)
                        position.x = ground.width - sprites.walk_right.frames[0].width;
                } else if (currentMap == MAP_1) {
                    if (position.x + sprites.walk_right.frames[0].width > ground.width)
                        position.x = ground.width - sprites.walk_right.frames[0].width;
                }

                // Gravidade
                velocity.y += gravity * dt;
                position.y += velocity.y * dt;

                // Colisão com plataformas
                isOnGround = false;
                float playerHeight = (float)sprites.walk_right.frames[0].height;

                for (int i = 0; i < PLATFORM_COUNT; i++) {
                    Rectangle plat = platforms[i];
                    if (velocity.y >= 0 &&
                        position.y + playerHeight >= plat.y &&
                        position.y + playerHeight - velocity.y * dt <= plat.y &&
                        position.x + sprites.walk_right.frames[0].width > plat.x &&
                        position.x < plat.x + plat.width) {

                        position.y = plat.y - playerHeight;
                        velocity.y = 0;
                        isOnGround = true;
                    }
                }

                if (position.y + playerHeight >= ground.y) {
                    position.y = ground.y - playerHeight;
                    velocity.y = 0;
                    isOnGround = true;
                }

                // Pulo
                if (isOnGround && IsKeyPressed(KEY_W)) {
                    velocity.y = jumpForce;
                    isOnGround = false;
                }

                // Ataque
                if (IsKeyPressed(KEY_L) && !attacking) {
                    attacking = true;
                    frame = 0;
                    timer = 0;
                    attackFacing = facing;
                }

                Animation currentAnim;
                if (attacking) {
                    currentAnim = (attackFacing == 1) ? sprites.attack_right : sprites.attack_left;
                } else if (moving) {
                    currentAnim = (facing == 1) ? sprites.walk_right : sprites.walk_left;
                } else {
                    currentAnim = (facing == 1) ? sprites.idle_right : sprites.idle_left;
                }

                if (currentAnim.frames != previousAnim.frames) {
                    frame = 0;
                    timer = 0;
                    previousAnim = currentAnim;
                }

                timer += dt;
                while (timer > sprites.frame_change) {
                    timer -= sprites.frame_change;
                    if (attacking) {
                        frame++;

                        if (frame == 1 && skeleton.alive) {
                            Rectangle playerRect = {position.x, position.y, (float)currentAnim.frames[frame].width, (float)currentAnim.frames[frame].height};
                            Texture2D skeletonTex = GetEnemyTexture(&skeleton, skeleton.sprites);
                            Rectangle skeletonRect = {skeleton.position.x, skeleton.position.y, (float)skeletonTex.width, (float)skeletonTex.height};

                            if (CheckCollisionRecs(playerRect, skeletonRect) &&
                                attackFacing == ((skeleton.position.x > position.x) ? 1 : -1)) {
                                DamageEnemy(&skeleton);

                                PlaySound(hitSound);
                            }
                        }

                        if (frame >= currentAnim.frame_count) {
                            frame = 0;
                            attacking = false;
                        }
                    } else {
                        frame = (frame + 1) % currentAnim.frame_count;
                    }
                }

                Texture2D current = currentAnim.frames[frame];

                // Atualiza inimigo
                Rectangle playerRect = {position.x, position.y, (float)current.width, (float)current.height};
                Texture2D enemyTex = GetEnemyTexture(&skeleton, skeleton.sprites);
                Rectangle enemyRect = GetEnemyRect(&skeleton, enemyTex);
                UpdateEnemy(&skeleton, position, dt, skeleton.sprites, playerRect);

                if (skeleton.alive && skeleton.attacking && playerHitTimer <= 0.0f) {
                    Rectangle attackArea = GetEnemyRect(&skeleton, enemyTex);

                    if (skeleton.facing >= 0) {
                        attackArea.width += 30;
                    } else {
                        attackArea.x -= 30;
                        attackArea.width += 30;
                    }

                    if (CheckCollisionRecs(playerRect, attackArea)) {
                        playerHealth -= 10;
                        playerHitTimer = PLAYER_HIT_COOLDOWN;

                        if (playerHealth <= 0) {
                            currentScreen = SCREEN_GAMEOVER;
                            break;
                        }

                        if (position.x < skeleton.position.x)
                            position.x -= 100 * dt;
                        else
                            position.x += 100 * dt;
                    }
                }


                UpdateCameraToFollowPlayer(&camera, position, 800, 600, ground.width, ground.y + ground.height);

                // Desenho
                BeginDrawing();
                ClearBackground(BLACK);
                BeginMode2D(camera);

                DrawTexture(background, 0, -490, WHITE);

                // Desenha o mapa conforme o currentMap
                if (currentMap == MAP_ORIGINAL) {
                    int tileWidth = groundSprites.frames[0].width;
                    int tiles = ground.width / tileWidth;
                    for (int i = 0; i < tiles + 1; i++) {
                        DrawTexture(groundSprites.frames[0], ground.x + i * tileWidth, ground.y, WHITE);
                    }
                    for (int i = 0; i < PLATFORM_COUNT; i++) {
                        DrawRectangleRec(platforms[i], GRAY);
                    }
                } else if (currentMap == MAP_1) {
                    DrawMap1(groundSprites.frames[0]);
                }

                DrawTexture(current, (int)position.x, (int)position.y, WHITE);

                if (skeleton.alive) {
                    DrawEnemy(&skeleton, enemyTex);
                }

                EndMode2D();

                DrawRectangle(20, 20, 300, 20, DARKGRAY);
                DrawRectangle(20, 20, 2 * playerHealth, 20, RED);
                DrawRectangleLines(20, 20, 300, 20, WHITE);

                EndDrawing();
            }

            UnloadPlayerSprites(sprites);
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

    
    UnloadSound(hitSound);
    CloseAudioDevice();
    CloseWindow();
    return 0;
}
