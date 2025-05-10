#include "raylib.h"
#include "sprites.h"
#include "camera.h"

#define PLATFORM_COUNT 4

int main() {
    InitWindow(800, 600, "Player Animation");
    SetTargetFPS(60);

    // Vida do jogador
    int playerHealth = 100;

    // Carregar sprites do jogador
    PlayerSprites sprites = LoadPlayerSprites("assets/sprites/player/player.json");

    // Carregar sprites do inimigo
    EnemySprites skeleton = LoadSkeletonGreenEnemySprites("assets/sprites/enemy/skeleton_green/skeleton_green.json");

    // Carregar sprites do chão
    GroundGrassSprites groundSprites = LoadGroundSprites("assets/sprites/map/ground.json");

    if (sprites.frame_change <= 0.0f) {
        sprites.frame_change = 0.10f;
    }

    if (skeleton.frame_change <= 0.0f) {
        skeleton.frame_change = 0.10f;
    }

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

    // Inicializar posição e movimento do inimigo
    Vector2 skeletonPosition = {600, 500};
    Vector2 skeletonVelocity = {50, 0};
    int skeletonFrame = 0;
    float skeletonTimer = 0;

    // Inicializa a câmera
    Camera2D camera = InitCamera(position, (Vector2){400, 300});

    while (!WindowShouldClose()) {
        float dt = GetFrameTime();
        bool moving = false;

        // Movimento do jogador
        if (IsKeyDown(KEY_D)) {
            position.x += speed * dt;
            facing = 1;
            moving = true;
        } else if (IsKeyDown(KEY_A)) {
            position.x -= speed * dt;
            facing = -1;
            moving = true;
        }

        // Limitar a posição do jogador dentro do terreno
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

        // Colisão com o chão
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

        // Escolher animação do jogador
        Animation currentAnim;
        if (attacking) {
            currentAnim = (attackFacing == 1) ? sprites.attack_right : sprites.attack_left;
        } else if (moving) {
            currentAnim = (facing == 1) ? sprites.walk_right : sprites.walk_left;
        } else {
            currentAnim = (facing == 1) ? sprites.idle_right : sprites.idle_left;
        }

        // Reset de frame se animação mudou
        if (currentAnim.frames != previousAnim.frames) {
            frame = 0;
            timer = 0;
            previousAnim = currentAnim;
        }

        // Atualizar frames do jogador
        timer += dt;
        while (timer > sprites.frame_change) {
            timer -= sprites.frame_change;
            if (attacking) {
                frame++;
                if (frame >= currentAnim.frame_count) {
                    frame = 0;
                    attacking = false;
                }
            } else {
                frame = (frame + 1) % currentAnim.frame_count;
            }
        }

        Texture2D current = currentAnim.frames[frame];

        // Atualizar inimigo
        skeletonPosition.x += skeletonVelocity.x * dt;
        if (skeletonPosition.x > 1000 || skeletonPosition.x < 100) {
            skeletonVelocity.x *= -1;
        }

        Animation skeletonAnim = (skeletonVelocity.x >= 0) ? skeleton.walk_right : skeleton.walk_left;

        skeletonTimer += dt;
        if (skeletonTimer > skeleton.frame_change) {
            skeletonTimer = 0;
            skeletonFrame = (skeletonFrame + 1) % skeletonAnim.frame_count;
        }

        Texture2D skeletonTexture = skeletonAnim.frames[skeletonFrame];

        // Colisão jogador vs inimigo
        Rectangle playerRect = {position.x, position.y, (float)current.width, (float)current.height};
        Rectangle skeletonRect = {skeletonPosition.x, skeletonPosition.y, (float)skeletonTexture.width, (float)skeletonTexture.height};

        if (CheckCollisionRecs(playerRect, skeletonRect)) {
            if (playerHealth > 0) playerHealth--;
            // Empurrar o jogador para fora
            if (position.x < skeletonPosition.x) {
                position.x -= 100 * dt;
            } else {
                position.x += 100 * dt;
            }
        }

        // Atualizar a câmera
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

        // Desenhar plataformas
        for (int i = 0; i < PLATFORM_COUNT; i++) {
            DrawRectangleRec(platforms[i], GRAY);
        }

        // Desenhar jogador
        DrawTexture(current, (int)position.x, (int)position.y, WHITE);

        // Desenhar inimigo
        DrawTexture(skeletonTexture, (int)skeletonPosition.x, (int)skeletonPosition.y, WHITE);

        EndMode2D();

        // Desenhar barra de vida
        DrawRectangle(20, 20, 200, 20, DARKGRAY);
        DrawRectangle(20, 20, 2 * playerHealth, 20, RED);
        DrawRectangleLines(20, 20, 200, 20, WHITE);

        EndDrawing();
    }

    UnloadPlayerSprites(sprites);
    UnloadSkeletonGreenEnemySprites(skeleton);
    UnloadGroundSprites(groundSprites);
    CloseWindow();
    return 0;
}
