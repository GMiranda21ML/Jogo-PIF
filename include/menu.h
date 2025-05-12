#ifndef MENU_H
#define MENU_H

typedef enum {
    SCREEN_MENU,
    SCREEN_GAME,
    SCREEN_EXIT
} GameScreen;

GameScreen RunMenu();

#endif
