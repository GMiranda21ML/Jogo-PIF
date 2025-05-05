#include "camera.h"

Camera2D InitCamera(Vector2 target, Vector2 offset) {
    Camera2D camera = { 0 };
    camera.target = target;
    camera.offset = offset;
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;
    return camera;
}

void UpdateCameraToFollowPlayer(Camera2D *camera, Vector2 playerPos, int screenWidth, int screenHeight) {
    camera->target = (Vector2){ playerPos.x + 32, playerPos.y + 32 }; 
}
