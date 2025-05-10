#include "raylib.h"
#include "sprites.h"
#include "camera.h"
#include <math.h>

#define PLATFORM_COUNT 4
#define DETECTION_RADIUS 200.0f
#define SKELETON_HIT_DURATION 0.2f

int main() {
    InitWindow(800, 600, "Player Animation");
    SetTargetFPS(60);

    int playerHealth = 100;

    PlayerSprites sprites = LoadPlayerSprites("assets/sprites/player/player.json");
    EnemySprites skeleton = LoadSkeletonGreenEnemySprites("assets/sprites/enemy/skeleton_green/skeleton_green.json");
    GroundGrassSprites groundSprites = LoadGroundSprites("assets/sprites/map/ground.json");

    if (sprites.frame_change <= 0.0f) sprites.frame_change = 0.10f;
    if (skeleton.frame_change <= 0.0f) skeleton.frame_change = 0.10f;

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

    Vector2 skeletonPosition = {600, 500};
    Vector2 skeletonVelocity = {0, 0};
    int skeletonFrame = 0;
    float skeletonTimer = 0;

    int skeletonHealth = 2;
    float skeletonHitTimer = 0;
    bool skeletonAlive = true;

    Camera2D camera = InitCamera(position, (Vector2){400, 300});

    while (!WindowShouldClose()) {
        float dt = GetFrameTime();
        bool moving = false;

        // Movimento jogador
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

        velocity.y += gravity * dt;
        position.y += velocity.y * dt;

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

        if (isOnGround && IsKeyPressed(KEY_W)) {
            velocity.y = jumpForce;
            isOnGround = false;
        }

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

                if (frame == 1 && skeletonAlive) {
                    Rectangle playerRect = {position.x, position.y, (float)currentAnim.frames[frame].width, (float)currentAnim.frames[frame].height};
                    Texture2D skeletonTexture = (skeletonVelocity.x >= 0) ? skeleton.walk_right.frames[skeletonFrame] : skeleton.walk_left.frames[skeletonFrame];
                    Rectangle skeletonRect = {skeletonPosition.x, skeletonPosition.y, (float)skeletonTexture.width, (float)skeletonTexture.height};

                    if (CheckCollisionRecs(playerRect, skeletonRect) &&
                        attackFacing == ((skeletonPosition.x > position.x) ? 1 : -1)) {
                        skeletonHealth--;
                        skeletonHitTimer = SKELETON_HIT_DURATION;
                        if (skeletonHealth <= 0) {
                            skeletonAlive = false;
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

        if (skeletonAlive) {
            skeletonPosition.y = ground.y - skeleton.walk_right.frames[0].height;

            float xDistance = fabs(position.x - skeletonPosition.x);
            bool playerDetected = xDistance < DETECTION_RADIUS;

            if (playerDetected) {
                skeletonVelocity.x = (skeletonPosition.x < position.x) ? 50 : -50;
            } else {
                skeletonVelocity.x = 0;
            }

            skeletonPosition.x += skeletonVelocity.x * dt;

            skeletonTimer += dt;
            if (skeletonTimer > skeleton.frame_change) {
                skeletonTimer = 0;
                skeletonFrame = (skeletonFrame + 1) % skeleton.walk_right.frame_count;
            }
        }

        if (skeletonHitTimer > 0) {
            skeletonHitTimer -= dt;
        }

        Rectangle playerRect = {position.x, position.y, (float)current.width, (float)current.height};
        Texture2D skeletonTexture = (skeletonVelocity.x >= 0) ? skeleton.walk_right.frames[skeletonFrame] : skeleton.walk_left.frames[skeletonFrame];
        Rectangle skeletonRect = {skeletonPosition.x, skeletonPosition.y, (float)skeletonTexture.width, (float)skeletonTexture.height};

        if (skeletonAlive && CheckCollisionRecs(playerRect, skeletonRect)) {
            if (playerHealth > 0) playerHealth--;
            if (position.x < skeletonPosition.x) {
                position.x -= 100 * dt;
            } else {
                position.x += 100 * dt;
            }
        }

        UpdateCameraToFollowPlayer(&camera, position, 800, 600, ground.width, ground.y + ground.height);

        BeginDrawing();
        ClearBackground(BLACK);
        BeginMode2D(camera);

        int tileWidth = groundSprites.frames[0].width;
        int tiles = ground.width / tileWidth;

        for (int i = 0; i < tiles + 1; i++) {
            DrawTexture(groundSprites.frames[0], ground.x + i * tileWidth, ground.y, WHITE);
        }

        for (int i = 0; i < PLATFORM_COUNT; i++) {
            DrawRectangleRec(platforms[i], GRAY);
        }

        DrawTexture(current, (int)position.x, (int)position.y, WHITE);

        if (skeletonAlive) {
            Color tint = (skeletonHitTimer > 0) ? RED : WHITE;
            DrawTexture(skeletonTexture, (int)skeletonPosition.x, (int)skeletonPosition.y, tint);
        }

        EndMode2D();

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
