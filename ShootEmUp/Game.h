#ifndef __GAME_H__
#define __GAME_H__

#pragma once

//includes

#include "SDL.h"
#include "SDL_ttf.h"
#include "SDL_mixer.h"
#include "SDL_thread.h"
#include <stdlib.h>
#include <string>
#include <vector>
#include "Imagework.h"
#include "Timer.h"
#include "GameObjects.h"
#include "Data.h"

void StartGame(SDL_Surface *link);		// запускает игру, отрисовывая её на поверхности link
void GameLoopAct();						// игровой цикл
void PressButton(int x);				// нажатие клавиши
void IngameClean();						// очистка мусора при окончании игры

class GameEntity {
private:
	int (*ThreadFnc)(void*);
public:
	GameEntity(SDL_Thread *thrd, SDL_Surface *scrn);

	static bool EndGame;
	static int bgScroll;
	static void StartGame();
	static SDL_Thread *Thread;

	// =============== Game Data =============
	static SDL_Surface *Screen;
	static SDL_Surface *LoadScreenBG;
	static SDL_Surface *SkySquare;
	
	static Unit *Hero;

	static int BulletCount;
	static Projectile *Bullet[100];
	static ProjectileType *BulletType[10];

	static SDL_Surface *Sprites[100];
	static SDL_Rect CharClips[100];
	static SDL_Rect BulletClips[100];
	static Mix_Music *BGM1;
	// =============== Game initialization functions ================
	static int InitGame(void *data);
	static void NewGame();
	static void ApplyBackground();
	static bool CleanUp();
	static bool SpriteClips();
	static bool PreloadMusic();
	static bool PreloadImages();
	static bool GenerateProjectileTypes();
	static void HandleButtonPress(int key);
	static void HandleButtonHold();
	// ================= Working with units ==================
	static void Shoot(Unit *unit);
	static void ProcessObject(GameObject *obj);
	static void ProcessUnit(Unit *unit);
	static void ProcessProjectile(Projectile *proj);
	// ================= Projectile explosions ===================
	static void Exp01(int target, int bullet);
};

#endif