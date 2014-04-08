#include "GameObjects.h"

GameObject::GameObject() {
	Image = 0;
	x = 0;
	y = 0;
	Health = 1;
	ClipNum = 0;
	width = Clip_ValuesB[ClipNum][0] - Clip_ValuesA[ClipNum][0];
	height = Clip_ValuesB[ClipNum][1] - Clip_ValuesA[ClipNum][1];
	Animation = 0;
	AnimationMax = 0;
}

GameObject::GameObject(int DataImageNumber, int PosX, int PosY, int hp) {
	Image = DataImageNumber;
	x = PosX;
	y = PosY;
	Health = hp;
	ClipNum = 0;
	width = Clip_ValuesB[ClipNum][0] - Clip_ValuesA[ClipNum][0];
	height = Clip_ValuesB[ClipNum][1] - Clip_ValuesA[ClipNum][1];
	Animation = 0;
	AnimationMax = 0;
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
ProjectileType::ProjectileType(int dmg, double vel, double acc, int spr1, int spr2, int hp) {
	Damage = dmg;
	dur = hp;
	Velocity = vel;
	Acceleration = acc;
	SpriteStart = spr1;
	SpriteEnd = spr2;
}
//===============================================================================
Projectile::Projectile(ProjectileType *type, int PosX, int PosY, double ang) {
	x = PosX;
	y = PosY;
	angle = ang;
	ProjType = type;
	Health = type->dur;
	Animation = 0;
	AnimationMax = type->SpriteEnd-type->SpriteStart;
	Player = false;
	curVel = ProjType->Velocity;
}
bool Projectile::Fly() {
	x += curVel*cos(angle*CONST_PI); 
	y += curVel*sin(angle*CONST_PI);
	curVel+=ProjType->Acceleration;

	if ((x < 0)||( x + PROJ_WIDTH > SCREEN_WIDTH )||(y < 0)||( y + PROJ_HEIGHT > SCREEN_HEIGHT )) {
		Health = 0;
	} else {
		return true;
	}
	return false;
}
void Kill() {

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
	Animation = 0;
	AnimationMax = 0;
	Cooldown = 0;

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