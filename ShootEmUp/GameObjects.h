#ifndef __GAMEOBJECTS_H__
#define __GAMEOBJECTS_H__

#pragma once
#include "Data.h"
#include <math.h>
#include <exception>

class GameObject {
private:
public:
	// ���������� �����
	int Image,		// �����, ����������� �� ������ ����� �������� ��� ����������� �������
		Animation,  // ����� ������������� ��������
		ClipNum;
	// ������� �����
	int Health,     // ���������� ��������� �������
		width,		// ������
		height;		// ������
	double 
		x, y,						// ����������
		xVelocity, yVelocity,		// �������� ����������� ������� �� ���� X � Y
		AccelX, AccelY,					// ��������� ������� �� ���� X � Y, 
		VelocityMax,					// ������ �������� ����������� �� ����� �� ����
	    DecelRate;						// �������� �������� �� ���� ��� ���������� ����

	// �������� 
	bool temporary;		// ������ �������� ��������� ������������
	int TempFxNum;		// ����� �� ���������� ���������� �����������
	//---------- ������� --------------------
	GameObject();
	GameObject(int DataImageNumber, int PosX, int PosY, int hp);	// ������� �����������
	void Move();													// ���������� ������ ������������ ����� �� ��������� � ����������
};

//==============================================================================================================

class ProjectileType {
public:
	int Damage;																		// �������� ���������� �������� �����
	double Velocity;														   // �������� �������� �������
	double Acceleration;											// ��������� ������� �� ���� X/Y
	int dur;
	int BulletDataNumber;											// ����� ���������������� ������ ���������� ������
	int GenerateCount; // ���������� ��������� ������������ ����� ���� ��� ��������
	double AngleSeparate; // �� ������� �������� ����� ���������� ���� �� ����� �����������, ���� �� ���������
	void (*OnHit)(int TargetId, int ProjectileId);								//	������ �� �������, �������������� ������������ � ������ ����� �������
	ProjectileType(int dmg, double vel, double acc, int data, int hp);	// �����������
};

//==============================================================================================================

class Unit: public GameObject {
public:
	int Charge,						// ������� ������� �����-�����������
		CurrentCooldown,
		Cooldown,					// ����� ����������� �������� �����
		Id;							// ������������� �����
	ProjectileType *BulletType;		// ������ �� ��� ������� �����
	bool IsEnemy;					// ��������, ����������� �� �������������� ����� � ����������
	

	// �������
	Unit(int DataImageNumber, int clip, int PosX, int PosY, int hp, int BltType);		// �����������
	void Damage(int value);						// �������, �������������� ��������� ������ �����
	void DirectionalAccel(bool left, bool right, bool up, bool down);	
	// �������, ���������� ��������� ����� ��� ������������ ������� ����������� �� ���� ��� ������a
	// * 1 - �����, 2 - ������, 3 - ����, 4 - �����

	// ������� ��������
	int PatternNumber,		// ����� ������ ��������� ��������
		PatternFrameCount;  // "������" ��� ��������� ��������
};

//==============================================================================================================

class Projectile: public GameObject {
private:
	double angle;		// ����, ��� ������� ������������ ������
	double curVel;
public:
	int Id;				// ������������� �������
	ProjectileType *ProjType;
	bool Player;
	Projectile(ProjectileType *type, int PosX, int PosY, double ang); // �����������
	bool Fly();			// �������, �������������� ����������� ������� ����� � ��������� ��������� � ���� �������
};

//==============================================================================================================

#endif

