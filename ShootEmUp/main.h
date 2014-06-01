#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "SDL_mixer.h"
#include <string>
#include <cstring>
#include <vector>
#include <direct.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include "SDL_thread.h"
#include "Imagework.h"
#include "Leaderboard.h"
#include "Game.h"
#include "Data.h"
#include <SDL_syswm.h>

// game
GameEntity *game;

// fonts
TTF_Font *FNT_BrushM = NULL;

// misc
SDL_Color textColor = { 0, 0, 0 };
bool GameStarted = false;
bool GameQuit = false;

//The surfaces
SDL_Surface *screen = NULL;
SDL_Surface *NEWGAMEbuttonsheet = NULL;
SDL_Surface *SETTINGSbuttonsheet = NULL;
SDL_Surface *LEADERbuttonsheet = NULL;
SDL_Surface *HELPbuttonsheet = NULL;
SDL_Surface *CLOSEbuttonsheet = NULL;
SDL_Surface *EXITbuttonsheet = NULL;
SDL_Surface *ButtonLeft = NULL;
SDL_Surface *ButtonRight = NULL;

SDL_Surface *MainMenuBG = NULL;
SDL_Surface *LeaderWindow = NULL;
SDL_Surface *LeaderMessagePart[10];

SDL_Surface *SettingsBoard = NULL;
SDL_Surface *SettingsWindow = NULL;
SDL_Surface *SettingsMessagePart[15];

SDL_Surface *HelpWindow = NULL;
SDL_Surface *HelpMessagePart[10];

// Music
Mix_Music *TitleBGM = NULL;
int MUSIC_VOLUME = 5;

// SFX
Mix_Chunk *BasicClick = NULL;
int SFX_VOLUME = 5;

//The event structure
char *LANG_FOLDER;
char *DATA_PATH;
SDL_Event ExitLoop;

// misc
int MSGcount = 0;
const int LB_SIZE = 8;
bool LBActivated = false;
bool STActivated = false;
bool HLPactivated = false;
SDL_Thread *SubThread = NULL;
SDL_Thread *GameThread = NULL;