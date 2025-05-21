#ifndef SCREENS_H
#define SCREENS_H

#include <raylib.h>

typedef enum GameScreen {
    SCREEN_GAME,
    SCREEN_MENU,
    SCREEN_EXIT,
    SCREEN_GAMEOVER, 
    SCREEN_KEYBOARD,
    SCREEN_CUTSCENE,
    SCREEN_EPILOGUE,
    SCREEN_ENDGAME,
    SCREEN_SCORE
} GameScreen;

typedef struct {
    const char *text;
    float displayTime;
} Dialogue;

#define NUM_IMAGES 3
#define NUM_DIALOGUES 5
#define TRANSITION_TIME 3.0f
#define IMAGE_DISPLAY_TIME 6.0f
#define FADE_OUT_TIME 3.0f
#define BEST_TIME_FILE "best_time.txt"

GameScreen RunMenu();
GameScreen RunGameOver();
GameScreen RunKeyboardScreen();
GameScreen RunCutscene();
GameScreen RunEpilogueCutscene(float totalGameTime);
GameScreen ShowEndGameScreen(Music *music, float totalGameTime);
GameScreen RunBestTimeScreen();

#endif
