#include "Game.h"
#include "GameObjects.h"

GameObject::GameObject() {
	Image = 0;
	x = 0;
	y = 0;
	Health = 1;
	ClipNum = 0;
}

GameObject::GameObject(int DataImageNumber, int PosX, int PosY, int hp, void *link) {
	Image = DataImageNumber;
	x = PosX;
	y = PosY;
	Health = hp;
	ClipNum = 0;
	GE = (GameEntity*)link;
}

void GameObject::Refresh() {
	apply_surface(x, y, GE->Sprites[Image], GE->Screen, &GE->CharClips[ClipNum]);
}

//===============================================================================
Unit::Unit(int DataImageNumber, int PosX, int PosY, int hp, int BltType, void *link) {
	x = PosX;
	y = PosY;
	Health = hp;
	Image = DataImageNumber;
	GE = (GameEntity*)link;
}