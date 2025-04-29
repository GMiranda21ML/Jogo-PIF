#include "raylib.h"
#include "sprites.h"

int main() {
    InitWindow(800, 600, "Player Animation");
    SetTargetFPS(60);

    PlayerSprites sprites = LoadPlayerSprites("assets/sprites/player/player.json");

    Rectangle ground = { 0, 550, 800, 100 }; // chão visível e com altura

    Vector2 position = {400, 500};
    Vector2 velocity = {0, 0};
    float gravity = 900.0f;
    float jumpForce = -400.0f;
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

        // Gravidade
        velocity.y += gravity * dt;
        position.y += velocity.y * dt;

        // Colisão com o chão
        float playerHeight = (float)sprites.walk_right.frames[0].height;
        if (position.y + playerHeight >= ground.y) {
            position.y = ground.y - playerHeight;
            velocity.y = 0;
            isOnGround = true;
        } else {
            isOnGround = false;
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

        // Desenhar
        BeginDrawing();
        ClearBackground(RAYWHITE);

        // Desenha chão
        DrawRectangleRec(ground, DARKGREEN);

        // Sprite atual
        Texture2D current =
            (facing == 1) ? sprites.walk_right.frames[frame] :
                            sprites.walk_left.frames[frame];

        DrawTexture(current, (int)position.x, (int)position.y, WHITE);

        EndDrawing();
    }

    UnloadPlayerSprites(sprites);
    CloseWindow();
    return 0;
}
