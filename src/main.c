#include "raylib.h"
#include "sprites.h"

#define PLATFORM_COUNT 4

int main() {
    InitWindow(800, 600, "Player Animation");
    SetTargetFPS(60);

    PlayerSprites sprites = LoadPlayerSprites("assets/sprites/player/player.json");

    if (sprites.frame_change <= 0.0f) {
        sprites.frame_change = 0.10f;
    }

    Rectangle ground = { 0, 550, 800, 100 };

    Rectangle platforms[PLATFORM_COUNT] = {
        {200, 450, 150, 20},
        {450, 350, 150, 20},
        {300, 250, 100, 20},
        {650, 200, 100, 20}
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

    while (!WindowShouldClose()) {
        float dt = GetFrameTime();
        bool moving = false;

        // Movimento livre mesmo durante ataque
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

        // Colisão com chão
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

        // Início do ataque
        if (IsKeyPressed(KEY_L) && !attacking) {
            attacking = true;
            frame = 0;
            timer = 0;
            attackFacing = facing;  // Travar direção da animação
        }

        // Escolher animação correta
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

        // Atualização de frames com tempo
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

        // Desenho
        BeginDrawing();
        ClearBackground(BLACK);

        DrawRectangleRec(ground, DARKGREEN);
        for (int i = 0; i < PLATFORM_COUNT; i++) {
            DrawRectangleRec(platforms[i], GRAY);
        }

        DrawTexture(current, (int)position.x, (int)position.y, WHITE);

        EndDrawing();
    }

    UnloadPlayerSprites(sprites);
    CloseWindow();
    return 0;
}
