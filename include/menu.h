#ifndef MENU_H
#define MENU_H

typedef enum GameScreen {
    SCREEN_GAME,
    SCREEN_MENU,
    SCREEN_EXIT,
    SCREEN_GAMEOVER, 
    SCREEN_KEYBOARD
} GameScreen;

GameScreen RunMenu();
GameScreen RunGameOver();
GameScreen RunKeyboardScreen();

#endif
