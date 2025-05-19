#include "sprites.h"
#include "cJSON.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static cJSON *LoadJSONFile(const char *path, char **outData) {
    FILE *file = fopen(path, "r");
    if (!file) return NULL;

    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    rewind(file);

    *outData = malloc(size + 1);
    fread(*outData, 1, size, file);
    (*outData)[size] = '\0';
    fclose(file);

    return cJSON_Parse(*outData);
}

static Animation LoadAnimationFromArray(cJSON *array) {
    if (!cJSON_IsArray(array)) return (Animation){0};

    int count = cJSON_GetArraySize(array);
    Texture2D *frames = malloc(sizeof(Texture2D) * count);
    for (int i = 0; i < count; i++) {
        cJSON *item = cJSON_GetArrayItem(array, i);
        const char *path = item->valuestring;
        frames[i] = FileExists(path) ? LoadTexture(path) : LoadTextureFromImage(GenImageColor(64, 64, RED));
    }

    return (Animation){ .frames = frames, .frame_count = count };
}

Texture2D FlipTextureHorizontally(Texture2D original) {
    Image img = LoadImageFromTexture(original);
    ImageFlipHorizontal(&img);
    Texture2D flipped = LoadTextureFromImage(img);
    UnloadImage(img);
    return flipped;
}

static Animation FlipAnimationHorizontally(Animation original) {
    Animation flipped = { .frame_count = original.frame_count };
    flipped.frames = malloc(sizeof(Texture2D) * original.frame_count);
    for (int i = 0; i < original.frame_count; i++) {
        flipped.frames[i] = FlipTextureHorizontally(original.frames[i]);
    }
    return flipped;
}

static void UnloadAnimation(Animation anim) {
    for (int i = 0; i < anim.frame_count; i++) UnloadTexture(anim.frames[i]);
    free(anim.frames);
}

PlayerSprites LoadPlayerSprites(const char *jsonPath) {
    PlayerSprites sprites = {0};
    char *data = NULL;
    cJSON *root = LoadJSONFile(jsonPath, &data);
    if (!root) return sprites;

    cJSON *walk = cJSON_GetObjectItem(root, "player_walk");
    cJSON *idle = cJSON_GetObjectItem(root, "player_idle");
    cJSON *attack = cJSON_GetObjectItem(root, "player_attack");
    
    if (walk) {
        sprites.walk_right = LoadAnimationFromArray(cJSON_GetObjectItem(walk, "right"));
        sprites.walk_left  = LoadAnimationFromArray(cJSON_GetObjectItem(walk, "left"));
    }
    if (idle) {
        sprites.idle_right = LoadAnimationFromArray(cJSON_GetObjectItem(idle, "right"));
        sprites.idle_left  = FlipAnimationHorizontally(sprites.idle_right);
    }
    if (attack) {
        sprites.attack_right = LoadAnimationFromArray(cJSON_GetObjectItem(attack, "right"));
        sprites.attack_left  = FlipAnimationHorizontally(sprites.attack_right);
    }

    cJSON *fc = cJSON_GetObjectItem(root, "frame_change");
    sprites.frame_change = fc ? (float)fc->valuedouble : 0.15f;

    cJSON_Delete(root);
    free(data);
    return sprites;
}

void UnloadPlayerSprites(PlayerSprites s) {
    UnloadAnimation(s.walk_right);
    UnloadAnimation(s.walk_left);
    UnloadAnimation(s.idle_right);
    UnloadAnimation(s.idle_left);
    UnloadAnimation(s.attack_right);
    UnloadAnimation(s.attack_left);
}

GroundGrassSprites LoadGroundSprites(const char *jsonPath) {
    GroundGrassSprites ground = {0};
    char *data = NULL;
    cJSON *root = LoadJSONFile(jsonPath, &data);
    if (!root) return ground;

    cJSON *groundArray = cJSON_GetObjectItem(cJSON_GetObjectItem(root, "ground_grass"), "ground");
    if (!groundArray || !cJSON_IsArray(groundArray)) {
        cJSON_Delete(root);
        free(data);
        return ground;
    }

    ground.frame_count = cJSON_GetArraySize(groundArray);
    ground.frames = malloc(sizeof(Texture2D) * ground.frame_count);
    for (int i = 0; i < ground.frame_count; i++) {
        const char *path = cJSON_GetArrayItem(groundArray, i)->valuestring;
        ground.frames[i] = FileExists(path) ? LoadTexture(path) : LoadTextureFromImage(GenImageColor(64, 64, PINK));
    }

    cJSON_Delete(root);
    free(data);
    return ground;
}

void UnloadGroundSprites(GroundGrassSprites ground) {
    for (int i = 0; i < ground.frame_count; i++) UnloadTexture(ground.frames[i]);
    free(ground.frames);
}

EnemySprites LoadEnemysSprites(const char *jsonPath) {
    EnemySprites sprites = {0};
    char *data = NULL;
    cJSON *root = LoadJSONFile(jsonPath, &data);
    if (!root) return sprites;

    cJSON *walk = cJSON_GetObjectItem(root, "enemy_walk");
    cJSON *attack = cJSON_GetObjectItem(root, "enemy_attack");

    if (walk) {
        sprites.walk_right = LoadAnimationFromArray(cJSON_GetObjectItem(walk, "right"));
        sprites.walk_left  = FlipAnimationHorizontally(sprites.walk_right);
    }
    if (attack) {
        sprites.attack_right = LoadAnimationFromArray(cJSON_GetObjectItem(attack, "right"));
        sprites.attack_left  = FlipAnimationHorizontally(sprites.attack_right);
    }

    cJSON *fc = cJSON_GetObjectItem(root, "frame_change");
    sprites.frame_change = fc ? (float)fc->valuedouble : 0.15f;

    cJSON_Delete(root);
    free(data);
    return sprites;
}

void UnloadEnemysSprites(EnemySprites s) {
    UnloadAnimation(s.walk_right);
    UnloadAnimation(s.walk_left);
    UnloadAnimation(s.attack_right);
    UnloadAnimation(s.attack_left);
}

EnemySprites LoadEnemySprites(const char *jsonPath) {
    return LoadEnemysSprites(jsonPath);
}

void UnloadEnemySprites(EnemySprites s) {
    UnloadEnemysSprites(s);
}