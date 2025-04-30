#include "raylib.h"
#include "sprites.h"

#define PLATFORM_COUNT 3

int main() {
    InitWindow(800, 600, "Player Animation");
    SetTargetFPS(60);

    PlayerSprites sprites = LoadPlayerSprites("assets/sprites/player/player.json");

    Rectangle ground = { 0, 550, 800, 100 }; // chão visível

    Rectangle platforms[PLATFORM_COUNT] = {
        {200, 450, 150, 20},
        {450, 350, 150, 20},
        {300, 250, 100, 20}
    };

    Vector2 position = {400, 500};
    Vector2 velocity = {0, 0};
    float gravity = 900.0f;
    float jumpForce = -450.0f;
    bool isOnGround = false;

    float speed = 200.0f;
    float timer = 0;
    int frame = 0;
    int facing = 1; // 1 = direita, -1 = esquerda

    while (!WindowShouldClose()) {
        float dt = GetFrameTime();
        bool moving = false;

        // Entrada horizontal
        if (IsKeyDown(KEY_D)) {
            position.x += speed * dt;
            facing = 1;
            moving = true;
        } else if (IsKeyDown(KEY_A)) {
            position.x -= speed * dt;
            facing = -1;
            moving = true;
        }

        // Aplicar gravidade
        velocity.y += gravity * dt;
        position.y += velocity.y * dt;

        // Verificar colisão com plataformas
        isOnGround = false;
        float playerHeight = (float)sprites.walk_right.frames[0].height;
        Texture2D current =
            (facing == 1) ? sprites.walk_right.frames[frame] :
                            sprites.walk_left.frames[frame];

        for (int i = 0; i < PLATFORM_COUNT; i++) {
            Rectangle plat = platforms[i];

            if (velocity.y >= 0 &&
                position.y + playerHeight >= plat.y &&
                position.y + playerHeight - velocity.y * dt <= plat.y &&
                position.x + current.width > plat.x &&
                position.x < plat.x + plat.width) {

                position.y = plat.y - playerHeight;
                velocity.y = 0;
                isOnGround = true;
            }
        }

        // Verificar colisão com o chão
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

        // Animação
        if (moving) {
            timer += dt;
            if (timer > sprites.frame_change) {
                frame = (frame + 1) % sprites.walk_right.frame_count;
                timer = 0;
            }
        } else {
            timer = 0;
            frame = 0;
        }

        // Desenho
        BeginDrawing();
        ClearBackground(BLACK);

        // Desenhar chão
        DrawRectangleRec(ground, DARKGREEN);

        // Desenhar plataformas
        for (int i = 0; i < PLATFORM_COUNT; i++) {
            DrawRectangleRec(platforms[i], GRAY);
        }

        // Desenhar jogador
        DrawTexture(current, (int)position.x, (int)position.y, WHITE);

        EndDrawing();
    }

    UnloadPlayerSprites(sprites);
    CloseWindow();
    return 0;
}
