#include <stdio.h>
#include "score.h"

float LoadBestTime() {
    FILE *file = fopen(SCORE_FILENAME, "r");
    if (!file) return -1.0f;

    float bestTime = -1.0f;
    fscanf(file, "%f", &bestTime);
    fclose(file);

    return bestTime;
}

void SaveBestTime(float bestTime) {
    FILE *file = fopen(SCORE_FILENAME, "w");
    if (!file) return;

    fprintf(file, "%.2f", bestTime);
    fclose(file);
}

void UpdateBestTime(float newTime) {
    float bestTime = LoadBestTime();

    if (bestTime < 0 || newTime < bestTime) {
        SaveBestTime(newTime);
    }
}
