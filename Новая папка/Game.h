#ifndef __GAME_H__
#define __GAME_H__

//includes

#include "SDL.h"
#include "SDL_ttf.h"
#include "SDL_mixer.h"
#include "SDL_thread.h"
#include "Imagework.h"
#include "Timer.h"
#include <stdlib.h>
#include <string>
#include <vector>

void StartGame(SDL_Surface *link);		// запускает игру, отрисовывая её на поверхности link
void GameLoopAct();						// игровой цикл
void PressButton(int x);				// нажатие клавиши
void IngameClean();						// очистка мусора при окончании игры

// button controls
const int BUTTON_UP = SDLK_w;
const int BUTTON_DOWN = SDLK_s;
const int BUTTON_LEFT = SDLK_a;
const int BUTTON_RIGHT = SDLK_d;
const int BUTTON_SHOOT = SDLK_SPACE;
const int BUTTON_ULTI = SDLK_RETURN;
const int BUTTON_QUIT = SDLK_ESCAPE;
const int FRAMES_PER_SECOND = 30;

class GameObject;
class Unit;
class Projectile;
class ProjectileType;

class GameEntity {
private:
	static void *thislink;
	int (*ThreadFnc)(void*);
public:
	GameEntity(SDL_Thread *thrd, SDL_Surface *scrn);

	static bool EndGame;
	static int bgScroll;
	void StartGame();

	// =============== Game Data =============
	static SDL_Surface *Screen;
	static SDL_Thread *Thread;

	static SDL_Surface *LoadScreenBG;
	static SDL_Surface *SkySquare;
	static SDL_Surface *Sprites[100];

	static Unit *Hero;
	static SDL_Rect CharClips[100];

	static Mix_Music *BGM1;
	// =============== Game initialization functions ================
	static int InitGame(void *data);
	static void NewGame();
	static void ApplyBackground();
	static bool CleanUp();
	static bool SpriteClips();
	static bool PreloadMusic();
	static bool PreloadImages();
	static void HandleInput();
};

#endif