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
#include <sstream>
#include "Imagework.h"
#include "Timer.h"
#include "GameObjects.h"
#include "Data.h"

typedef void (*Type_PatternFunc)(Unit *target);

void StartGame(SDL_Surface *link);		// запускает игру, отрисовывая её на поверхности link
void GameLoopAct();						// игровой цикл
void PressButton(int x);				// нажатие клавиши
void IngameClean();						// очистка мусора при окончании игры

class GameEntity {
private:
	int (*ThreadFnc)(void*);
public:
	GameEntity(SDL_Thread *thrd, SDL_Surface *scrn);
	static std::string LanguageFolder;

	static bool EndGame;
	static int bgScroll;
	static void StartGame();
	static SDL_Thread *Thread;

	// =============== Game Data =============
	static SDL_Surface *Screen;
	static SDL_Surface *LoadScreenBG;
	static SDL_Surface *SkySquare;
	static SDL_Surface *InterfaceOverlay;
	static SDL_Surface *HpBar;
	static SDL_Surface *EnergyBar;
	static SDL_Surface *GrayBar;
	static SDL_Surface *VictoryScreen;
	static SDL_Surface *DefeatScreen;
	
	static Unit *Hero;
	static Unit *Enemies[100];
	static bool EnemyAlive[100];
	static bool Victory;
	static bool Defeat;

	static int BulletCount;
	static int FxCount;
	static int SpawnTimer;
	static Projectile *Bullet[100];
	static ProjectileType *BulletType[10];
	static SpriteData *BulletData[100];

	static SpriteData *FxData[100];
	static GameObject *SpecialFx[30];

	static SDL_Surface *Sprites[100];
	static SDL_Rect CharClips[100];
	static Mix_Music *BGM1;
	static Mix_Music *BGM2;
	static Mix_Music *BGM3;
	static Mix_Chunk *SFX[10];
	static int BossNumber;
	static int Score;
	static std::stringstream TextStream;
	static SDL_Surface *ScoreSurface;
	static TTF_Font *FNT_Forte;
	static SDL_Color FNT_WhiteColor;
	static SDL_Color FNT_BlackColor;
	static std::string PlayerName;
	static bool KEYHOLD_SHIFT;
	static bool STATE_Pause;
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
	static void GenerateOverlay();
	// ================= Working with units ==================
	static void Shoot(Unit *unit);
	static void ProcessObject(GameObject *obj);
	static void ProcessUnit(Unit *unit);
	static void ProcessProjectile(Projectile *proj);
	static bool IntersectionUnits(Unit *unit1, Unit *unit2);
	static bool IntersectionProjectile(Unit *unit, Projectile *proj);
	// patterns
	static Type_PatternFunc PatternFunc[10];
	static void Pattern01(Unit *target);
	static void Pattern02(Unit *target);
	static void Pattern03(Unit *target);
	static void Pattern04(Unit *target);
	static void Pattern05(Unit *target);
	static void Pattern06(Unit *target);
	static void Pattern07(Unit *target);
	static void EnemySpawn(int timer);
	static void AddEnemy1(int x, int y, int pattern);
	static void AddEnemy2(int x, int y, int pattern);
	static void AddEnemy3(int x, int y, int pattern);
	static int EnemyCount;
	static void WinGame(int timer);
	static void LoseGame(int timer);
	// ================= Projectile explosions ===================
	static void Exp01(int target, int bullet);
	static void Exp02(int target, int bullet);
	static void Exp03(int target, int bullet);
	static void Exp04(int target, int bullet);
	// ================= Special FX ==============================
	static void CreateExplosion(int x, int y, int type);
};

#endif