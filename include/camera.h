#ifndef CAMERA_H
#define CAMERA_H

#include "raylib.h"

Camera2D InitCamera(Vector2 target, Vector2 offset);

void UpdateCameraToFollowPlayer(Camera2D *camera, Vector2 playerPos, int screenWidth, int screenHeight);

#endif
