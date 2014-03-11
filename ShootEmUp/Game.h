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

void StartGame(SDL_Surface *link);		// ��������� ����, ����������� � �� ����������� link
void GameLoopAct();						// ������� ����
void PressButton(int x);				// ������� �������
void IngameClean();						// ������� ������ ��� ��������� ����

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
	// ���������� �����
	int Image,		// �����, ����������� �� ������ ����� �������� ��� ����������� �������
		Animation,  // ����� ������������� ��������
		ClipNum;
	// ������� �����
	int Health,     // ���������� ��������� �������
		x,			// ���������� � �������
		y;			// ���������� Y �������
	double xVelocity, yVelocity,		// �������� ����������� ������� �� ���� X � Y
		AccelX, AccelY,					// ��������� ������� �� ���� X � Y, 
		VelocityMax;					// ������ �������� ����������� �� ����� �� ����
	// �������� 
	bool temporary;		// ������ �������� ��������� ������������
	//---------- ������� --------------------
	GameObject();
	GameObject(int DataImageNumber, int PosX, int PosY, int hp);	// ������� �����������
	void Refresh();													// ��������� ������ �� ������, ������������� ���
	void SetAnimation(int anim);									// ���������� �������� �� ��������� ����
	void Move();													// ���������� ������ ������������ ����� �� ��������� � ����������
};

class ProjectileType {
private:
	int Damage;																		// �������� ���������� �������� �����
	double VelocityX, VelocityY;													// �������� �������� ������� �� ���� X/Y
	double AccelerationX, AccelerationY;											// ��������� ������� �� ���� X/Y
public:
	void (*OnHit)(int TargetId, int ProjectileId);								//	������ �� �������, �������������� ������������ � ������ ����� �������
	ProjectileType(int dmg, double velx, double vely, double accx, double accy);	// �����������
};

class Unit: public GameObject {
private:
	int CurrentCooldown;			// ������� ��������� ������� �����������
public:
	int Charge,						// ������� ������� �����-�����������
		Cooldown,					// ����� ����������� �������� �����
		Id;							// ������������� �����
	ProjectileType *BulletType;		// ������ �� ��� ������� �����
	bool IsEnemy;					// ��������, ����������� �� �������������� ����� � ����������
	

	// �������
	Unit(int DataImageNumber, int PosX, int PosY, int hp, int BltType);		// �����������
	void Shoot();					// �������, �������������� �������� ������ ������� ����� ������ � ����������� �����������
	void Kill();					// �������, �������������� �������� �����, ��� ������������ �������, ��� � ������
	void Remove();					// ������� ����� �� ����, �� ����������� ��� ��� ����������� (����� �� ������� ������)
	void Damage(int value);			// �������, �������������� ��������� ������ �����

	// ������� ��������
	int (*Pattern)(void *data);		// �������, ������� ������������ ������������ �������� �����
	SDL_Thread *PatternThread;		// �����, � ������� ����������� ������������������ �������� � ��������
};

class Projectile: public GameObject {
private:
	double angle;		// ����, ��� ������� ������������ ������
	int Id;				// ������������� �������
public:
	Projectile(ProjectileType *type, int PosX, int PosY, double ang); // �����������
	void Fly();			// �������, �������������� ����������� ������� ����� � ��������� ��������� � ���� �������
	void Kill();		// �������, �������������� ������ �������
};

#endif