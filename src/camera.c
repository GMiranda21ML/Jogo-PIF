#include "camera.h"

Camera2D InitCamera(Vector2 target, Vector2 offset) {
    Camera2D camera;
    camera.target = target;
    camera.offset = offset;
    camera.rotation = 0.0f;
    camera.zoom = 1.3f;
    return camera;
}

void UpdateCameraToFollowPlayer(Camera2D *camera, Vector2 playerPos, int screenWidth, int screenHeight, float levelWidth, float levelHeight) {
    Vector2 newTarget;
    newTarget.x = playerPos.x + 32;
    newTarget.y = playerPos.y - 165;

    float viewWidth = screenWidth / camera->zoom;
    float viewHeight = screenHeight / camera->zoom;

    float halfViewWidth = viewWidth / 2.0f;
    float halfViewHeight = viewHeight / 2.0f;

    if (newTarget.x < halfViewWidth) {
        newTarget.x = halfViewWidth;
    }
    if (newTarget.x > levelWidth - halfViewWidth) {
        newTarget.x = levelWidth - halfViewWidth;
    }

    if (newTarget.y < halfViewHeight) {
        newTarget.y = halfViewHeight;
    }
    if (newTarget.y > levelHeight - halfViewHeight) {
        newTarget.y = levelHeight - halfViewHeight;
    }

    camera->target = newTarget;
}
