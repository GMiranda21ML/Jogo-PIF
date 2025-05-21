#include <stdlib.h>
#include "raylib.h"
#include "levelUp.h"
#include "player.h"

void InitPlayerLevels(PlayerLevel* player) {
    player->currentKills = 0;

    LevelNode* level1 = (LevelNode *)malloc(sizeof(LevelNode));
    level1->level = 1;
    level1->damage = 5;
    level1->spritePath = "assets/sprites/player/playerLevel1.json";

    LevelNode* level2 = (LevelNode *)malloc(sizeof(LevelNode));
    level2->level = 2;
    level2->damage = 7;
    level2->spritePath = "assets/sprites/player/playerLevel2.json";

    LevelNode* level3 = (LevelNode *)malloc(sizeof(LevelNode));
    level3->level = 3;
    level3->damage = 10;
    level3->spritePath = "assets/sprites/player/playerLevel3.json";

    level1->next = level2;
    level2->next = level3;
    level3->next = NULL;

    player->head = level1;
    player->currentLevel = level1;
}

void AddKill(PlayerLevel* playerLevel, Sound levelUpSound, Player *player) {
    playerLevel->currentKills++;

    if (playerLevel->currentKills % 6 == 0 && playerLevel->currentLevel->next != NULL) {
        playerLevel->currentLevel = playerLevel->currentLevel->next;

        player->playerHealth = 150;

        PlaySound(levelUpSound);
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
