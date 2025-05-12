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

    // redenrizar e inverter sprites idle
    cJSON *idle = cJSON_GetObjectItem(root, "player_idle");
    if (!idle) {
        fprintf(stderr, "Erro: player_idle não encontrado no JSON.\n");
        cJSON_Delete(root);
        free(data);
        return sprites;
    }

    sprites.idle_right = LoadAnimationFromArray(cJSON_GetObjectItem(idle, "right"));

    // Criar idle_left invertendo idle_right
    sprites.idle_left.frame_count = sprites.idle_right.frame_count;
    sprites.idle_left.frames = malloc(sizeof(Texture2D) * sprites.idle_left.frame_count);
    for (int i = 0; i < sprites.idle_left.frame_count; i++) {
        sprites.idle_left.frames[i] = FlipTextureHorizontally(sprites.idle_right.frames[i]);
    }

    // renderizar e inverter attack
    cJSON *attack = cJSON_GetObjectItem(root, "player_attack");
    if (!attack) {
        fprintf(stderr, "Erro: player_attack não encontrado no JSON.\n");
        cJSON_Delete(root);
        free(data);
        return sprites;
    }

    sprites.attack_right = LoadAnimationFromArray(cJSON_GetObjectItem(attack, "right"));
    
    // Criar attack_left invertendo attack_right
    sprites.attack_left.frame_count = sprites.attack_right.frame_count;
    sprites.attack_left.frames = malloc(sizeof(Texture2D) * sprites.attack_left.frame_count);
    for (int i = 0; i < sprites.attack_left.frame_count; i++) {
        sprites.attack_left.frames[i] = FlipTextureHorizontally(sprites.attack_right.frames[i]);
    }


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
    for (int i = 0; i < sprites.idle_right.frame_count; i++) {
        UnloadTexture(sprites.idle_right.frames[i]);
    }
    for (int i = 0; i < sprites.idle_left.frame_count; i++) {
        UnloadTexture(sprites.idle_left.frames[i]);
    }
    for (int i = 0; i < sprites.attack_right.frame_count; i++) {
        UnloadTexture(sprites.attack_right.frames[i]);
    }
    for (int i = 0; i < sprites.attack_left.frame_count; i++) {
        UnloadTexture(sprites.attack_left.frames[i]);
    }

    free(sprites.walk_right.frames);
    free(sprites.walk_left.frames);
    free(sprites.idle_right.frames);
    free(sprites.idle_left.frames);
    free(sprites.attack_right.frames);
    free(sprites.attack_left.frames);
}

Texture2D FlipTextureHorizontally(Texture2D original) {
    Image img = LoadImageFromTexture(original);
    ImageFlipHorizontal(&img);
    Texture2D flipped = LoadTextureFromImage(img);
    UnloadImage(img);
    return flipped;
}

GroundGrassSprites LoadGroundSprites(const char *jsonPath) {
    GroundGrassSprites ground = {0};

    FILE *file = fopen(jsonPath, "r");
    if (!file) {
        fprintf(stderr, "Erro ao abrir JSON do chão: %s\n", jsonPath);
        return ground;
    }

    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    rewind(file);

    char *data = malloc(size + 1);
    fread(data, 1, size, file);
    data[size] = '\0';
    fclose(file);

    cJSON *root = cJSON_Parse(data);
    free(data);
    if (!root) {
        fprintf(stderr, "Erro ao parsear JSON do chão.\n");
        return ground;
    }

    cJSON *ground_grass = cJSON_GetObjectItem(root, "ground_grass");
    if (!ground_grass) {
        fprintf(stderr, "Erro: ground_grass não encontrado no JSON.\n");
        cJSON_Delete(root);
        return ground;
    }

    cJSON *groundArray = cJSON_GetObjectItem(ground_grass, "ground");
    if (!groundArray || !cJSON_IsArray(groundArray)) {
        fprintf(stderr, "Erro: ground não é um array.\n");
        cJSON_Delete(root);
        return ground;
    }

    int count = cJSON_GetArraySize(groundArray);
    ground.frame_count = count;
    ground.frames = malloc(sizeof(Texture2D) * count);

    for (int i = 0; i < count; i++) {
        cJSON *item = cJSON_GetArrayItem(groundArray, i);
        const char *path = item->valuestring;
        if (FileExists(path)) {
            ground.frames[i] = LoadTexture(path);
        } else {
            ground.frames[i] = LoadTextureFromImage(GenImageColor(64, 64, PINK)); // fallback
            fprintf(stderr, "Erro: %s não encontrado\n", path);
        }
    }

    cJSON_Delete(root);
    return ground;
}

void UnloadGroundSprites(GroundGrassSprites ground) {
    for (int i = 0; i < ground.frame_count; i++) {
        UnloadTexture(ground.frames[i]);
    }
    free(ground.frames);
}

EnemySprites LoadSkeletonGreenEnemySprites(const char *jsonPath) {
    EnemySprites sprites = {0};

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

    cJSON *walk = cJSON_GetObjectItem(root, "skeleton_green_walk");
    if (walk) {
        sprites.walk_right = LoadAnimationFromArray(cJSON_GetObjectItem(walk, "right"));
        sprites.walk_left = LoadAnimationFromArray(cJSON_GetObjectItem(walk, "left"));
    } else {
        fprintf(stderr, "Erro: skeleton_green_walk não encontrado.\n");
    }

    cJSON *fc = cJSON_GetObjectItem(root, "frame_change");
    sprites.frame_change = fc ? (float)fc->valuedouble : 0.15f;

    cJSON *attack = cJSON_GetObjectItem(root, "skeleton_green_attack");
    if (attack) {
        sprites.attack_right = LoadAnimationFromArray(cJSON_GetObjectItem(attack, "right"));
        
        sprites.attack_left.frame_count = sprites.attack_right.frame_count;
        sprites.attack_left.frames = malloc(sizeof(Texture2D) * sprites.attack_left.frame_count);
        for (int i = 0; i < sprites.attack_left.frame_count; i++) {
            sprites.attack_left.frames[i] = FlipTextureHorizontally(sprites.attack_right.frames[i]);
        }
    } else {
        fprintf(stderr, "Erro: skeleton_green_attack não encontrado.\n");
    }

    cJSON_Delete(root);
    free(data);
    return sprites;
}

void UnloadSkeletonGreenEnemySprites(EnemySprites sprites) {
    for (int i = 0; i < sprites.walk_right.frame_count; i++) {
        UnloadTexture(sprites.walk_right.frames[i]);
    }
    for (int i = 0; i < sprites.walk_left.frame_count; i++) {
        UnloadTexture(sprites.walk_left.frames[i]);
    }
    for (int i = 0; i < sprites.attack_right.frame_count; i++) {
        UnloadTexture(sprites.attack_right.frames[i]);
    }
    for (int i = 0; i < sprites.attack_left.frame_count; i++) {
        UnloadTexture(sprites.attack_left.frames[i]);
    }

    free(sprites.walk_right.frames);
    free(sprites.walk_left.frames);
    free(sprites.attack_right.frames);
    free(sprites.attack_left.frames);
}


EnemySprites LoadEnemySprites(const char *jsonPath) {
    return LoadSkeletonGreenEnemySprites(jsonPath);
}

void UnloadEnemySprites(EnemySprites sprites) {
    UnloadSkeletonGreenEnemySprites(sprites);
}

