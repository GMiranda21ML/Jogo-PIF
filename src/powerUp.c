#include <stdlib.h>
#include "powerUp.h"

void InitPlayerLevels(PlayerLevel* player) {
    player->currentKills = 0;

    LevelNode* level1 = malloc(sizeof(LevelNode));
    level1->level = 1;
    level1->damage = 1;
    level1->spritePath = "assets/sprites/player/playerLevel1.json";

    LevelNode* level2 = malloc(sizeof(LevelNode));
    level2->level = 2;
    level2->damage = 2;
    level2->spritePath = "assets/sprites/player/playerLevel2.json";

    LevelNode* level3 = malloc(sizeof(LevelNode));
    level3->level = 3;
    level3->damage = 3;
    level3->spritePath = "assets/sprites/player/playerLevel3.json";

    level1->next = level2;
    level2->next = level3;
    level3->next = NULL;

    player->head = level1;
    player->currentLevel = level1;
}

void AddKill(PlayerLevel* player) {
    player->currentKills++;

    if (player->currentKills % 3 == 0 && player->currentLevel->next != NULL) {
        player->currentLevel = player->currentLevel->next;
    }
}

void FreePlayerLevels(PlayerLevel* player) {
    LevelNode* current = player->head;
    while (current != NULL) {
        LevelNode* next = current->next;
        free(current);
        current = next;
    }
}
