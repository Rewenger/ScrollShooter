#include "GameObjects.h"

using namespace std;

GameObject::GameObject() {
	Image = 0;
	x = 0;
	y = 0;
	Health = 1;
	ClipNum = 0;
	width = Clip_ValuesB[ClipNum][0] - Clip_ValuesA[ClipNum][0];
	height = Clip_ValuesB[ClipNum][1] - Clip_ValuesA[ClipNum][1];
	Animation = 0;
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
}

void GameObject::Move() {
	x+= xVelocity; 
	if (!CollisionIgnore) {
		if (x < 0) {
			x = 0;
		} else if (x + width > SCREEN_WIDTH) {
			x = SCREEN_WIDTH - width;
		}
	}

	y += yVelocity; 
	if (!CollisionIgnore) {
		if (y < 0) {
			y = 0;
		} else if (y + height > SCREEN_HEIGHT) {
			y = SCREEN_HEIGHT - height;
		}
	}
}
//===============================================================================
ProjectileType::ProjectileType(int dmg, double vel, double acc, int data, int hp) {
	Damage = dmg;
	dur = hp;
	Velocity = vel;
	Acceleration = acc;
	BulletDataNumber = data;
}
//===============================================================================
Projectile::Projectile(ProjectileType *type, int PosX, int PosY, double ang) {
	if (type == nullptr)
		throw exception();
	x = PosX;
	y = PosY;
	angle = ang;
	ProjType = type;
	Health = type->dur;
	Animation = 0;
	//AnimationMax = type->SpriteEnd-type->SpriteStart;
	Player = false;
	curVel = ProjType->Velocity;
	width = PROJ_WIDTH;
	height = PROJ_HEIGHT;
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
//===============================================================================
Unit::Unit(int DataImageNumber, int clip, int PosX, int PosY, int hp, int BltType) {
	x = PosX;
	y = PosY;
	Health = hp;
	Image = DataImageNumber;
	ClipNum = clip;
	width = Clip_ValuesB[ClipNum][0];
	height = Clip_ValuesB[ClipNum][1];
	Animation = 0;
	Cooldown = 0;

	xVelocity = 0; 
	yVelocity = 0;
	VelocityMax = 7.0;

	AccelX = 0.7;
	AccelY = 0.7;
	DecelRate = 0.7; 
	
	temporary = false;
	Cooldown = 0;
	CurrentCooldown = 0;
	IsEnemy = true;
	PatternNumber = 0;
	PatternFrameCount = 0;
}

void Unit::Damage(int value) {
	Health-=value;
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