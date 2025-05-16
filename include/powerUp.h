#ifndef POWERUP_H
#define POWERUP_H

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
void AddKill(PlayerLevel* player);
void FreePlayerLevels(PlayerLevel* player);

#endif
