#include "raylib.h"
#include "sprites.h"

int main() {
    InitWindow(800, 600, "Player Animation");
    SetTargetFPS(60);

    PlayerSprites sprites = LoadPlayerSprites("assets/sprites/player/player.json");

    Vector2 position = {100, 300};
    float speed = 2.5f;
    float timer = 0;
    int frame = 0;
    int facing = 1; // 1 = right, -1 = left

    while (!WindowShouldClose()) {
        float dt = GetFrameTime();

        // Movimento
        if (IsKeyDown(KEY_RIGHT)) {
            position.x += speed;
            facing = 1;
            timer += dt;
        } else if (IsKeyDown(KEY_LEFT)) {
            position.x -= speed;
            facing = -1;
            timer += dt;
        } else {
            timer = 0;
            frame = 0;
        }

        // Trocar frame
        if (timer > sprites.frame_change) {
            frame = (frame + 1) % sprites.walk_right.frame_count;
            timer = 0;
        }

        // Desenhar
        BeginDrawing();
        ClearBackground(RAYWHITE);

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
