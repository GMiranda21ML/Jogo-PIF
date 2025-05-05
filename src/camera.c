#include "camera.h"

Camera2D InitCamera(Vector2 target, Vector2 offset) {
    Camera2D camera = { 0 };
    camera.target = target;
    camera.offset = offset;
    camera.rotation = 0.0f;
    camera.zoom = 1.3f;
    return camera;
}

void UpdateCameraToFollowPlayer(Camera2D *camera, Vector2 playerPos, int screenWidth, int screenHeight, float levelWidth, float levelHeight) {
    camera->target = (Vector2){ playerPos.x + 32, playerPos.y - 130 };

    float viewWidth = screenWidth / camera->zoom;
    float viewHeight = screenHeight / camera->zoom;

    float halfViewWidth = viewWidth / 2.0f;
    float halfViewHeight = viewHeight / 2.0f;

    if (camera->target.x < halfViewWidth) camera->target.x = halfViewWidth;
    if (camera->target.x > levelWidth - halfViewWidth) camera->target.x = levelWidth - halfViewWidth;

    if (camera->target.y < halfViewHeight) camera->target.y = halfViewHeight;
    if (camera->target.y > levelHeight - halfViewHeight) camera->target.y = levelHeight - halfViewHeight;
}
