#include "map_original.h"
#include <stdlib.h>

#define ORIGINAL_MAP_PLATFORM_COUNT 4

static Rectangle originalPlatforms[ORIGINAL_MAP_PLATFORM_COUNT];
static Rectangle ground;

void InitMapOriginal(void) {
    originalPlatforms[0] = (Rectangle){200, 450, 150, 20};
    originalPlatforms[1] = (Rectangle){450, 350, 150, 20};
    originalPlatforms[2] = (Rectangle){300, 250, 100, 20};
    originalPlatforms[3] = (Rectangle){685, 250, 100, 20};

    ground = (Rectangle){0, 550, 2000, 500};
}

Rectangle* GetMapOriginalPlatforms(void) {
    return originalPlatforms;
}

int GetMapOriginalPlatformCount(void) {
    return ORIGINAL_MAP_PLATFORM_COUNT;
}

Rectangle GetMapOriginalGround(void) {
    return ground;
}

void DrawMapOriginal(void) {
    for (int i = 0; i < ORIGINAL_MAP_PLATFORM_COUNT; i++) {
        DrawRectangleRec(originalPlatforms[i], GRAY);
    }
}
