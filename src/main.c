#include "raylib.h"
#include "sprites.h"

#define PLATFORM_COUNT 3

int main() {
    InitWindow(800, 600, "Player Animation");
    SetTargetFPS(60);

    PlayerSprites sprites = LoadPlayerSprites("assets/sprites/player/player.json");

    Rectangle ground = { 0, 550, 800, 100 };

    Rectangle platforms[PLATFORM_COUNT] = {
        {200, 450, 150, 20}, // {x, y, largura, altura} || x, y onde vai estar posicionado
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
    int facing = 1;

    while (!WindowShouldClose()) {
        float dt = GetFrameTime();
        bool moving = false;

        // Entrada de movimento
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

        static Animation previousAnim = {0};  // Fora do while se quiser manter estado

        // Selecionar animação
        Animation currentAnim;
        if (moving) {
            if (facing == 1) {
                currentAnim = sprites.walk_right;
            } else {
                currentAnim = sprites.walk_left;
            }
        } else {
            if (facing == 1) {
                currentAnim = sprites.idle_right;
            } else {
                currentAnim = sprites.idle_left;
            }
        }
        
        // Resetar frame se a animação mudou
        if (currentAnim.frames != previousAnim.frames) {
            frame = 0;
            timer = 0;
            previousAnim = currentAnim;
        }
        
        // Atualizar quadro da animação
        timer += dt;
        if (timer > sprites.frame_change) {
            frame = (frame + 1) % currentAnim.frame_count;
            timer = 0;
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
