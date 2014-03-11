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

int InitGame(void *data);

class GameEntity {
private:
	int (*ThreadFnc)(void*);
public:
	bool EndGame;
	int bgScroll;
	SDL_Surface *Screen;
	GameEntity(SDL_Thread *thrd, SDL_Surface *scrn);
	void StartGame();
	SDL_Thread *Thread;
};

class GameObject {
private:
public:
	// визуальная часть
	int Image,		// число, указывающее на нужный набор спрайтов для отображения объекта
		Animation,  // номер проигрываемой анимации
		ClipNum;
	// игровая часть
	int Health,     // оставшаяся прочность объекта
		x,			// координата Х объекта
		y;			// координата Y объекта
	double xVelocity, yVelocity,		// скорость перемещения объекта по осям X и Y
		AccelX, AccelY,					// ускорение объекта по осям X и Y, 
		VelocityMax;					// предел скорости перемещения по любой из осей
	// свойства 
	bool temporary;		// объект является временным спецэффектом
	//---------- Функции --------------------
	GameObject();
	GameObject(int DataImageNumber, int PosX, int PosY, int hp);	// базовый конструктор
	void Refresh();													// обновляет объект на экране, прорисовывает его
	void SetAnimation(int anim);									// перемещает анимацию на указанный кадр
	void Move();													// заставляет объект перемещаться вперёд со скоростью и ускорением
};

class ProjectileType {
private:
	int Damage;																		// величина наносимого снарядом урона
	double VelocityX, VelocityY;													// скорость движения снаряда по осям X/Y
	double AccelerationX, AccelerationY;											// ускорение снаряда по осям X/Y
public:
	void (*OnHit)(int TargetId, int ProjectileId);								//	ссылка на функцию, обрабатывающую столкновение с данным типом снаряда
	ProjectileType(int dmg, double velx, double vely, double accx, double accy);	// конструктор
};

class Unit: public GameObject {
private:
	int CurrentCooldown;			// текущее состояние таймера перезарядки
public:
	int Charge,						// степень зарядки супер-способности
		Cooldown,					// время перезарядки выстрела юнита
		Id;							// идентификатор юнита
	ProjectileType *BulletType;		// ссылка на тип снаряда юнита
	bool IsEnemy;					// параметр, указывающий на принадлежность юнита к оппонентам
	

	// функции
	Unit(int DataImageNumber, int PosX, int PosY, int hp, int BltType);		// конструктор
	void Shoot();					// функция, осуществляющая создание нового снаряда перед юнитом и запускающая перезарядку
	void Kill();					// функция, обрабатывающее убийство юнита, как управляемого игроком, так и врагом
	void Remove();					// удаляет юнита из игры, не обрабатывая это как уничтожение (вышел за пределы экрана)
	void Damage(int value);			// функция, обрабатывающая получение юнитом урона

	// паттерн движения
	int (*Pattern)(void *data);		// функция, которая осуществляет предписанное движение юнита
	SDL_Thread *PatternThread;		// поток, в котором выполняется последовательность движений в паттерне
};

class Projectile: public GameObject {
private:
	double angle;		// угол, под которым перемещается снаряд
	int Id;				// идентификатор снаряда
public:
	Projectile(ProjectileType *type, int PosX, int PosY, double ang); // конструктор
	void Fly();			// функция, осуществляющая перемещение снаряда вперёд и обработку связанных с этим событий
	void Kill();		// функция, обрабатывающая смерть снаряда
};

#endif