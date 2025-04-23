#include "sprites.h"
#include "cJSON.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static Animation LoadAnimationFromArray(cJSON *array) {
    if (!cJSON_IsArray(array)) {
        fprintf(stderr, "Erro: item não é array!\n");
        return (Animation){0};
    }

    int count = cJSON_GetArraySize(array);
    Texture2D *frames = malloc(sizeof(Texture2D) * count);
    if (!frames) {
        fprintf(stderr, "Erro: não foi possível alocar memória para as texturas.\n");
        return (Animation){0};
    }

    for (int i = 0; i < count; i++) {
        cJSON *item = cJSON_GetArrayItem(array, i);
        if (!cJSON_IsString(item)) {
            fprintf(stderr, "Erro: caminho da imagem não é string no índice %d\n", i);
            continue;
        }

        const char *path = item->valuestring;
        if (!FileExists(path)) {
            fprintf(stderr, "Erro: imagem não encontrada: %s\n", path);
            frames[i] = LoadTextureFromImage(GenImageColor(64, 64, RED)); // fallback visual
        } else {
            frames[i] = LoadTexture(path);
            printf("✔️ Carregado: %s\n", path);
        }
    }

    return (Animation){ .frames = frames, .frame_count = count };
}

PlayerSprites LoadPlayerSprites(const char *jsonPath) {
    PlayerSprites sprites = {0};

    FILE *file = fopen(jsonPath, "r");
    if (!file) {
        fprintf(stderr, "Erro ao abrir JSON: %s\n", jsonPath);
        return sprites;
    }

    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    rewind(file);

    char *data = malloc(size + 1);
    if (!data) {
        fprintf(stderr, "Erro de alocação para dados do JSON.\n");
        fclose(file);
        return sprites;
    }

    fread(data, 1, size, file);
    data[size] = '\0';
    fclose(file);

    cJSON *root = cJSON_Parse(data);
    if (!root) {
        fprintf(stderr, "Erro ao fazer parse do JSON.\n");
        free(data);
        return sprites;
    }

    cJSON *walk = cJSON_GetObjectItem(root, "player_walk");
    if (!walk) {
        fprintf(stderr, "Erro: player_walk não encontrado no JSON.\n");
        cJSON_Delete(root);
        free(data);
        return sprites;
    }

    sprites.walk_right = LoadAnimationFromArray(cJSON_GetObjectItem(walk, "right"));
    sprites.walk_left = LoadAnimationFromArray(cJSON_GetObjectItem(walk, "left"));

    cJSON *fc = cJSON_GetObjectItem(root, "frame_change");
    sprites.frame_change = fc ? (float)fc->valuedouble : 0.15f;

    cJSON_Delete(root);
    free(data);
    return sprites;
}

void UnloadPlayerSprites(PlayerSprites sprites) {
    for (int i = 0; i < sprites.walk_right.frame_count; i++) {
        UnloadTexture(sprites.walk_right.frames[i]);
    }
    for (int i = 0; i < sprites.walk_left.frame_count; i++) {
        UnloadTexture(sprites.walk_left.frames[i]);
    }

    free(sprites.walk_right.frames);
    free(sprites.walk_left.frames);
}
