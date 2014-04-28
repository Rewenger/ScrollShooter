#ifndef __GAMEOBJECTS_H__
#define __GAMEOBJECTS_H__

#pragma once
#include "Data.h"
#include <math.h>
#include <exception>

class GameObject {
private:
public:
	// визуальная часть
	int Image,		// число, указывающее на нужный набор спрайтов для отображения объекта
		Animation,  // номер проигрываемой анимации
		AnimationMax,
		ClipNum;
	// игровая часть
	int Health,     // оставшаяся прочность объекта
		width,		// ширина
		height;		// высота
	double 
		x, y,						// координаты
		xVelocity, yVelocity,		// скорость перемещения объекта по осям X и Y
		AccelX, AccelY,					// ускорение объекта по осям X и Y, 
		VelocityMax,					// предел скорости перемещения по любой из осей
	    DecelRate;						// снижение скорости по осям при отсутствии силы

	// свойства 
	bool temporary;		// объект является временным спецэффектом
	//---------- Функции --------------------
	GameObject();
	GameObject(int DataImageNumber, int PosX, int PosY, int hp);	// базовый конструктор
	void SetAnimation(int anim);									// перемещает анимацию на указанный кадр
	void Move();													// заставляет объект перемещаться вперёд со скоростью и ускорением
};

//==============================================================================================================

class ProjectileType {
public:
	int Damage;																		// величина наносимого снарядом урона
	double Velocity;														   // скорость движения снаряда
	double Acceleration;											// ускорение снаряда по осям X/Y
	int dur;
	int SpriteStart, SpriteEnd;													// начальный и конечный спрайт анимации на файле спрайтов снарядов
	int GenerateCount; // количество созданных прожектайлов этого типа при выстреле
	double AngleSeparate; // на сколько градусов будут отделяться друг от друга прожектайлы, если их несколько
	void (*OnHit)(int TargetId, int ProjectileId);								//	ссылка на функцию, обрабатывающую столкновение с данным типом снаряда
	ProjectileType(int dmg, double vel, double acc, int spr1, int spr2, int hp);	// конструктор
};

//==============================================================================================================

class Unit: public GameObject {
public:
	int Charge,						// степень зарядки супер-способности
		CurrentCooldown,
		Cooldown,					// время перезарядки выстрела юнита
		Id;							// идентификатор юнита
	ProjectileType *BulletType;		// ссылка на тип снаряда юнита
	bool IsEnemy;					// параметр, указывающий на принадлежность юнита к оппонентам
	

	// функции
	Unit(int DataImageNumber, int clip, int PosX, int PosY, int hp, int BltType);		// конструктор
	void Kill();								// функция, обрабатывающее убийство юнита, как управляемого игроком, так и врагом
	void Remove();								// удаляет юнита из игры, не обрабатывая это как уничтожение (вышел за пределы экрана)
	void Damage(int value);						// функция, обрабатывающая получение юнитом урона
	void DirectionalAccel(bool left, bool right, bool up, bool down);	
	// функция, изменяющая положение юнита при задерживании клавиши направления за один тик таймерa
	// * 1 - влево, 2 - вправо, 3 - вниз, 4 - вверх

	// паттерн движения
	//int (*Pattern)(void *data);		// функция, которая осуществляет предписанное движение юнита
	//SDL_Thread *PatternThread;		// поток, в котором выполняется последовательность движений в паттерне
};

//==============================================================================================================

class Projectile: public GameObject {
private:
	double angle;		// угол, под которым перемещается снаряд
	double curVel;
public:
	int Id;				// идентификатор снаряда
	ProjectileType *ProjType;
	bool Player;
	Projectile(ProjectileType *type, int PosX, int PosY, double ang); // конструктор
	bool Fly();			// функция, осуществляющая перемещение снаряда вперёд и обработку связанных с этим событий
	void Kill();		// функция, обрабатывающая смерть снаряда
};

//==============================================================================================================

#endif

