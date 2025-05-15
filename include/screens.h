#ifndef SCREENS_H
#define SCREENS_H

typedef enum GameScreen {
    SCREEN_GAME,
    SCREEN_MENU,
    SCREEN_EXIT,
    SCREEN_GAMEOVER, 
    SCREEN_KEYBOARD,
    SCREEN_CUTSCENE
} GameScreen;

GameScreen RunMenu();
GameScreen RunGameOver();
GameScreen RunKeyboardScreen();
GameScreen RunCutscene();

#endif
