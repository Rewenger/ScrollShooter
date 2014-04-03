#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "SDL_mixer.h"
#include "Game.h"
#include <string>
#include <vector>
#include <direct.h>
#include "SDL_thread.h"
#include "Imagework.h"
#include "Leaderboard.h"

// game
GameEntity *game;


// fonts
TTF_Font *FNT_BrushM = NULL;

// misc
SDL_Color textColor = { 0, 0, 0 };
bool GameStarted = false;
bool GameQuit = false;

//Screen attributes
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int SCREEN_BPP = 32;



//The surfaces
SDL_Surface *screen = NULL;
SDL_Surface *NEWGAMEbuttonsheet = NULL;
SDL_Surface *SETTINGSbuttonsheet = NULL;
SDL_Surface *LEADERbuttonsheet = NULL;
SDL_Surface *CLOSEbuttonsheet = NULL;
SDL_Surface *EXITbuttonsheet = NULL;

SDL_Surface *MainMenuBG = NULL;
SDL_Surface *LeaderWindow = NULL;
SDL_Surface *LeaderMessagePart[10];

SDL_Surface *SettingsWindow = NULL;
SDL_Surface *SettingsMessagePart[10];

// Music
Mix_Music *TitleBGM = NULL;

// SFX
Mix_Chunk *BasicClick = NULL;

//The event structure
SDL_Event ExitLoop;

// misc
int MSGcount = 0;
const int LB_SIZE = 8;
bool LBActivated = false;
bool STActivated = false;
SDL_Thread *SubThread = NULL;
SDL_Thread *GameThread = NULL;