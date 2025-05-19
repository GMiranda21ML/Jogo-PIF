#ifndef LEVELUP_H
#define LEVELUP_H

#include "raylib.h"

typedef struct Player Player;

typedef struct LevelNode {
    int level;
    int damage;
    const char* spritePath;
    struct LevelNode* next;
} LevelNode;

typedef struct {
    LevelNode* head;
    int currentKills;
    LevelNode* currentLevel;
} PlayerLevel;


void InitPlayerLevels(PlayerLevel* player);
void AddKill(PlayerLevel* playerLevel, Sound levelUpSound, Player *player);
void FreePlayerLevels(PlayerLevel* player);

#endif
