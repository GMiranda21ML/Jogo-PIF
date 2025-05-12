#include "raylib.h"
#include "sprites.h"
#include "camera.h"
#include "enemy.h"
#include "menu.h"
#include <math.h>

#define PLATFORM_COUNT 4
#define DETECTION_RADIUS 200.0f
#define SKELETON_HIT_DURATION 0.2f

int main() {
    InitWindow(800, 600, "Player Animation");
    InitAudioDevice();
    SetTargetFPS(60);

    GameScreen currentScreen = RunMenu();
    if (currentScreen == SCREEN_EXIT) {
        CloseWindow();
        return 0;
    }

    int playerHealth = 1;
    float playerHitTimer = 0.0f;
    const float PLAYER_HIT_COOLDOWN = 0.5f;

    PlayerSprites sprites = LoadPlayerSprites("assets/sprites/player/player.json");
    GroundGrassSprites groundSprites = LoadGroundSprites("assets/sprites/map/ground.json");

    if (sprites.frame_change <= 0.0f) sprites.frame_change = 0.10f;

    Rectangle ground = { 0, 550, 2000, 500 };

    Rectangle platforms[PLATFORM_COUNT] = {
        {200, 450, 150, 20},
        {450, 350, 150, 20},
        {300, 250, 100, 20},
        {685, 250, 100, 20}
    };

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

    while (!WindowShouldClose()) {
        float dt = GetFrameTime();
        bool moving = false;

        if (playerHitTimer > 0.0f) playerHitTimer -= dt;

        if (IsKeyDown(KEY_D)) {
            position.x += speed * dt;
            facing = 1;
            moving = true;
        } else if (IsKeyDown(KEY_A)) {
            position.x -= speed * dt;
            facing = -1;
            moving = true;
        }

        if (position.x < 0) position.x = 0;
        if (position.x + sprites.walk_right.frames[0].width > ground.width)
            position.x = ground.width - sprites.walk_right.frames[0].width;

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
                        skeleton.health--;
                        skeleton.hitTimer = SKELETON_HIT_DURATION;
                        if (skeleton.health <= 0) {
                            skeleton.alive = false;
                        }
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
        
        // Verifica dano ao jogador
        Rectangle playerRect = {position.x, position.y, (float)current.width, (float)current.height};
        Texture2D enemyTex = GetEnemyTexture(&skeleton, skeleton.sprites);
        Rectangle enemyRect = {skeleton.position.x, skeleton.position.y, (float)enemyTex.width, (float)enemyTex.height};
        UpdateEnemy(&skeleton, position, dt, skeleton.sprites, playerRect);

        if (skeleton.alive && CheckCollisionRecs(playerRect, enemyRect) && playerHitTimer <= 0.0f) {
            if (playerHealth > 0) {
                playerHealth--;
                if (playerHealth <= 0) {
                    currentScreen = SCREEN_GAMEOVER;
                    break;
                }
            }
            
            playerHitTimer = PLAYER_HIT_COOLDOWN;

            // Knockback
            if (position.x < skeleton.position.x)
                position.x -= 100 * dt;
            else
                position.x += 100 * dt;
        }

        UpdateCameraToFollowPlayer(&camera, position, 800, 600, ground.width, ground.y + ground.height);

        // Desenho
        BeginDrawing();
        ClearBackground(BLACK);
        BeginMode2D(camera);

        // Desenhar chão
        int tileWidth = groundSprites.frames[0].width;
        int tiles = ground.width / tileWidth;

        for (int i = 0; i < tiles + 1; i++) {
            DrawTexture(groundSprites.frames[0], ground.x + i * tileWidth, ground.y, WHITE);
        }

        // Plataformas
        for (int i = 0; i < PLATFORM_COUNT; i++) {
            DrawRectangleRec(platforms[i], GRAY);
        }

        // Jogador
        DrawTexture(current, (int)position.x, (int)position.y, WHITE);

        // Inimigo
        if (skeleton.alive) {
            Color tint = (skeleton.hitTimer > 0) ? RED : WHITE;
            DrawTexture(enemyTex, (int)skeleton.position.x, (int)skeleton.position.y, tint);
        }

        EndMode2D();

        // Vida do jogador
        DrawRectangle(20, 20, 200, 20, DARKGRAY);
        DrawRectangle(20, 20, 2 * playerHealth, 20, RED);
        DrawRectangleLines(20, 20, 200, 20, WHITE);

        EndDrawing();
    }

    if (currentScreen == SCREEN_GAMEOVER) {
        GameScreen next = RunGameOver();
        if (next == SCREEN_MENU) {
            UnloadPlayerSprites(sprites);
            UnloadEnemySprites(skeleton.sprites);
            UnloadGroundSprites(groundSprites);
            CloseAudioDevice();
            CloseWindow();
            main();
        } else {
            CloseWindow();
            return 0;
        }
    }    

    UnloadPlayerSprites(sprites);
    UnloadEnemySprites(skeleton.sprites);
    UnloadGroundSprites(groundSprites);
    CloseAudioDevice();
    CloseWindow();
    return 0;
}
