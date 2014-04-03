#include "GameObjects.h"

GameObject::GameObject() {
	Image = 0;
	x = 0;
	y = 0;
	Health = 1;
	ClipNum = 0;
	width = Clip_ValuesB[ClipNum][0] - Clip_ValuesA[ClipNum][0];
	height = Clip_ValuesB[ClipNum][1] - Clip_ValuesA[ClipNum][1];
}

GameObject::GameObject(int DataImageNumber, int PosX, int PosY, int hp) {
	Image = DataImageNumber;
	x = PosX;
	y = PosY;
	Health = hp;
	ClipNum = 0;
	width = Clip_ValuesB[ClipNum][0] - Clip_ValuesA[ClipNum][0];
	height = Clip_ValuesB[ClipNum][1] - Clip_ValuesA[ClipNum][1];
}

void GameObject::Move() {
	x+= xVelocity; 
	if (x < 0) {
		x = 0;
	} else if (x + width > SCREEN_WIDTH) {
		x = SCREEN_WIDTH - width;
	}

	y += yVelocity; 
	if (y < 0) {
		y = 0;
	} else if (y + height > SCREEN_HEIGHT) {
		y = SCREEN_HEIGHT - height;
	}
}
//===============================================================================
Unit::Unit(int DataImageNumber, int clip, int PosX, int PosY, int hp, int BltType) {
	x = PosX;
	y = PosY;
	Health = hp;
	Image = DataImageNumber;
	ClipNum = clip;
	width = Clip_ValuesB[ClipNum][0] - Clip_ValuesA[ClipNum][0];
	height = Clip_ValuesB[ClipNum][1] - Clip_ValuesA[ClipNum][1];
	//bullettype = btype;
	Animation = 0;

	xVelocity = 0; 
	yVelocity = 0;
	VelocityMax = 5.5;

	AccelX = 0.35;
	AccelY = 0.35;
	DecelRate = 0.30; 
	
	temporary = false;
	Cooldown = 0;
	IsEnemy = true;

}

void Unit::DirectionalAccel(bool left, bool right, bool up, bool down) {
	if (right) {
		if (xVelocity < VelocityMax) {
			xVelocity+=AccelX;
		} else {
			xVelocity = VelocityMax;
		}
	} else {
		if (left) {
			if (xVelocity > -VelocityMax) {
				xVelocity-=AccelX;
			} else {
				xVelocity = -VelocityMax;
			}
		} else {
			if (xVelocity != 0) {
				if (abs(xVelocity) <= DecelRate) 
					xVelocity = 0;
				else if (xVelocity > 0) 
					xVelocity-=DecelRate;
				else 
					xVelocity+=DecelRate;
			}
		}
	}

	if (down) {
		if (yVelocity < VelocityMax) {
			yVelocity+=AccelY;
		} else {
			yVelocity = VelocityMax;
		}
	} else {
		if (up) {
			if (yVelocity > -VelocityMax) {
				yVelocity-=AccelY;
			} else {
				yVelocity = -VelocityMax;
			}
		} else {
			if (yVelocity != 0) {
				if (abs(yVelocity) <= DecelRate) 
					yVelocity = 0;
				else if (yVelocity > 0) 
					yVelocity-=DecelRate;
				else 
					yVelocity+=DecelRate;
			}
		}
	}
}