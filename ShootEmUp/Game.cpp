#include "Game.h"

//unsigned char test::X = 1;
bool GameEntity::EndGame = false;
bool GameEntity::Victory = false;
bool GameEntity::Defeat = false;
bool GameEntity::KEYHOLD_SHIFT = false;
int GameEntity::bgScroll = 0;
int GameEntity::BulletCount = 0;
int GameEntity::FxCount = 0;
int GameEntity::SpawnTimer = 0;
int GameEntity::EnemyCount = 0;
int GameEntity::BossNumber;
int GameEntity::Score = 0;
int GameEntity::ExplosionCount = -1;
bool GameEntity::STATE_Pause = false;
std::string GameEntity::PlayerName = "";
std::stringstream GameEntity::TextStream;
SDL_Thread *GameEntity::Thread = NULL;
SDL_Surface *GameEntity::Screen = NULL;
SDL_Surface *GameEntity::LoadScreenBG = NULL;
SDL_Surface *GameEntity::SkySquare = NULL;
SDL_Surface *GameEntity::InterfaceOverlay = NULL;
SDL_Surface *GameEntity::Sprites[100];
SDL_Surface *GameEntity::HpBar;
SDL_Surface *GameEntity::EnergyBar;
SDL_Surface *GameEntity::GrayBar;
SDL_Surface *GameEntity::VictoryScreen;
SDL_Surface *GameEntity::DefeatScreen;
SDL_Surface *GameEntity::ScoreSurface;
TTF_Font *GameEntity::FNT_Forte = NULL;
SDL_Color GameEntity::FNT_WhiteColor = { 255, 255, 255 };
SDL_Color GameEntity::FNT_BlackColor = { 0, 0, 0 };

Unit *GameEntity::Hero;
Unit *GameEntity::Enemies[100];
bool GameEntity::EnemyAlive[100];
GameObject *GameEntity::SpecialFx[30];
Projectile *GameEntity::Bullet[100];
ProjectileType *GameEntity::BulletType[10];
Type_PatternFunc GameEntity::PatternFunc[10];
std::string GameEntity::LanguageFolder;

SpriteData *GameEntity::FxData[100];
SpriteData *GameEntity::BulletData[100];
SDL_Rect GameEntity::CharClips[100];
Mix_Music *GameEntity::BGM1;
Mix_Music *GameEntity::BGM2;
Mix_Music *GameEntity::BGM3;
Mix_Chunk *GameEntity::SFX[10];
//===============================================================================
// check whether unit intersects with another unit
bool GameEntity::IntersectionUnits(Unit *unit1, Unit *unit2) {
	return (unit1->x <= unit2->x+unit2->width &&
          unit2->x <= unit1->x+unit1->width &&
          unit1->y <= unit2->y+unit2->height &&
          unit2->y <= unit1->y+unit1->height);
}

bool GameEntity::IntersectionSquare(Unit *unit, int x, int y, int w, int h) {
	return (unit->x <= x+w &&
          x <= unit->x+unit->width &&
          unit->y <= y+h &&
          y <= unit->y+unit->height);
}

// check whether unit intersects with projectile
bool GameEntity::IntersectionProjectile(Unit *unit, Projectile *proj) {
		return (unit->x <= proj->x+proj->width &&
          proj->x <= unit->x+unit->width &&
          unit->y <= proj->y+proj->width &&
          proj->y <= unit->y+unit->height);
}
// explosion of bullet #1 - hero bullet
void GameEntity::Exp01(int target, int bullet) {
	if (Defeat)
		return;
	Enemies[target]->Damage(Bullet[bullet]->ProjType->Damage);
	if (Enemies[target]->Health <= 0) {
		if (target != BossNumber) {
			CreateExplosion(Enemies[target]->x-100, Enemies[target]->y-100, 3);
			Score++;
		} else {
			Enemies[target]->Health = 9999;
			SpawnTimer = 0;
			Victory = true;
			Score+=40;
			STATE_Pause = true;
		}
	}
	Bullet[bullet]->Health--;
	CreateExplosion(Bullet[bullet]->x-100, Bullet[bullet]->y-100, 2);
}
// explosion of bullet #2 - enemy basic bullet
void GameEntity::Exp02(int target, int bullet) {
	if (Victory)
		return;
	Hero->Damage(Bullet[bullet]->ProjType->Damage);
	if (Hero->Health <= 0) {
		CreateExplosion(Hero->x-100, Hero->y-100, 3);
		Defeat = true;
		SpawnTimer = 0;
	}
	Bullet[bullet]->Health--;
	CreateExplosion(Bullet[bullet]->x-100, Bullet[bullet]->y-100, 2);
}
// explosion of bullet #3 - boss basic bullet
void GameEntity::Exp03(int target, int bullet) {
	if (Victory)
		return;
	Hero->Damage(Bullet[bullet]->ProjType->Damage);
	if (Hero->Health <= 0) {
		CreateExplosion(Hero->x-100, Hero->y-100, 3);
		Defeat = true;
		SpawnTimer = 0;
	}
	Bullet[bullet]->Health--;
	CreateExplosion(Bullet[bullet]->x-100, Bullet[bullet]->y-100, 3);
}
// explosion of bullet #4 - boss advanced bullet
void GameEntity::Exp04(int target, int bullet) {
	if (Victory)
		return;
	Hero->Damage(Bullet[bullet]->ProjType->Damage);
	if (Hero->Health <= 0) {
		CreateExplosion(Hero->x-100, Hero->y-100, 3);
		Defeat = true;
		SpawnTimer = 0;
	}
	Bullet[bullet]->Health--;
	CreateExplosion(Bullet[bullet]->x-50, Bullet[bullet]->y-50, 4);
}
// explosion of bullet #5 - hero special attack
void GameEntity::Exp05(int target, int bullet) {
	if (Defeat)
		return;

	for (int i = 0; i < 100; i++) {
		if (EnemyAlive[i]) {
			if (IntersectionSquare(Enemies[i], Bullet[bullet]->x-30, Bullet[bullet]->y-90, 170, 170)) {
				Enemies[i]->Damage(Bullet[bullet]->ProjType->Damage);
				if (Enemies[i]->Health <= 0) {
					if (i != BossNumber) {
						CreateExplosion(Enemies[i]->x-100, Enemies[i]->y-100, 3);
						Score++;
					} else {
						Enemies[i]->Health = 9999;
						SpawnTimer = 0;
						Victory = true;
						Score+=40;
						STATE_Pause = true;
					}
				}
			}
		}
	}
	Bullet[bullet]->Health--;
	CreateExplosion(Bullet[bullet]->x-75, Bullet[bullet]->y-75, 5);
	CreateExplosion(Bullet[bullet]->x-125, Bullet[bullet]->y-75, 5);
	CreateExplosion(Bullet[bullet]->x-75, Bullet[bullet]->y-125, 5);
	CreateExplosion(Bullet[bullet]->x-125, Bullet[bullet]->y-125, 5);
}

void GameEntity::ExplosionWave(int count) {
	if (Defeat) {
		ExplosionCount = -1;
		return;
	}

	if (count%5 == 0) {
		int shift = -200;
		for (int a = 0; a < 3; a++) {
			shift+=100;
			for (int i = 0; i < 100; i++) {
				if (EnemyAlive[i]) {
					if (IntersectionSquare(Enemies[i], Hero->x+shift-45, Hero->y-90-count*15, 170, 170)) {
						Enemies[i]->Damage(3);
						if (Enemies[i]->Health <= 0) {
							if (i != BossNumber) {
								CreateExplosion(Enemies[i]->x-100, Enemies[i]->y-100, 3);
								Score++;
							} else {
								Enemies[i]->Health = 9999;
								SpawnTimer = 0;
								Victory = true;
								Score+=40;
								STATE_Pause = true;
							}
						}
					}
				}
			}
			CreateExplosion(Hero->x+shift-45, Hero->y-75-count*15, 5);
		}
	}
	if (count >= 75)
		ExplosionCount = -1;
}


// movement pattern #1
// moves diagonally downright, follows right, diagonally right-upwards and disappears
void GameEntity::Pattern01(Unit *target) {
	target->PatternFrameCount++;
	int x = target->PatternFrameCount;
	if (x < 30) {
		target->DirectionalAccel(false, true, false, true);
	} else if ((x > 30)&&(x < 200)) {
		target->DirectionalAccel(false, true, false, false);
	} else if ((x > 200)&&(x < 230)) {
		target->DirectionalAccel(false, true, true, false);
	} else if (x > 230) {
		target->Health = 0;
	}

	if (x%60 == 0)
		Shoot(target);
}
// movement pattern #2
// moves diagonally downleft, follows left, diagonally left-upwards and disappears
void GameEntity::Pattern02(Unit *target) {
	target->PatternFrameCount++;
	int x = target->PatternFrameCount;
	if (x < 30) {
		target->DirectionalAccel(true, false, false, true);
	} else if ((x > 30)&&(x < 200)) {
		target->DirectionalAccel(true, false, false, false);
	} else if ((x > 200)&&(x < 230)) {
		target->DirectionalAccel(true, false, true, false);
	} else if (x > 230) {
		target->Health = 0;
	}

	if (x%60 == 0)
		Shoot(target);
}
// movement pattern #3
// moves horizontally left and disappears
void GameEntity::Pattern03(Unit *target) {
	target->PatternFrameCount++;
	int x = target->PatternFrameCount;
	if (x < 160) {
		target->DirectionalAccel(true, false, false, false);
	} else if (x > 160) {
		target->Health = 0;
	}

	if (x%35 == 0)
		Shoot(target);
}
// movement pattern #4
// moves horizontally right and disappears
void GameEntity::Pattern04(Unit *target) {
	target->PatternFrameCount++;
	int x = target->PatternFrameCount;
	if (x < 160) {
		target->DirectionalAccel(false, true, false, false);
	} else if (x > 160) {
		target->Health = 0;
	}
	if (x%35 == 0)
		Shoot(target);
}

// movement pattern #5
// moves diagonally right-down and disappears
void GameEntity::Pattern05(Unit *target) {
	target->PatternFrameCount++;
	int x = target->PatternFrameCount;
	if (x < 160) {
		target->DirectionalAccel(false, true, false, true);
	} else if (x > 160) {
		target->Health = 0;
	}
	if (x%30 == 0)
		Shoot(target);
}

// movement pattern #6
// moves diagonally left-down and disappears
void GameEntity::Pattern06(Unit *target) {
	target->PatternFrameCount++;
	int x = target->PatternFrameCount;
	if (x < 160) {
		target->DirectionalAccel(true, false, false, true);
	} else if (x > 160) {
		target->Health = 0;
	}
	if (x%30 == 0)
		Shoot(target);
}
// movement pattern #7
// pattern for a type-C (boss)
void GameEntity::Pattern07(Unit *target) {
	target->PatternFrameCount++;
	int x = target->PatternFrameCount;
	if (x < 50) {
		target->DirectionalAccel(false, false, false, true);
	} else if ((x < 150) && (x > 50)) {
		target->DirectionalAccel(false, true, false, false);
	} else if ((x-150)%500 < 250) {
		target->DirectionalAccel(true, false, false, false);
	} else {
		target->DirectionalAccel(false, true, false, false);
	}
	if (x%45 == 0)
		Shoot(target);
	if (x%70 == 0) {
		target->BulletType = BulletType[4];
		Shoot(target);
		target->BulletType = BulletType[3];
	}
}

void GameEntity::WinGame(int timer) {
	int ex, ey;
	if ((timer < 150)&&(timer%30 == 0)) {
		ex = rand()%120, ey = rand()%120;
		CreateExplosion(Enemies[BossNumber]->x-ex, Enemies[BossNumber]->y-ey, 3);
	} else if ((timer < 300)&&(timer%5 == 0)&&(timer > 150)) {
		ex = rand()%150, ey = rand()%175;
		CreateExplosion(Enemies[BossNumber]->x-ex, Enemies[BossNumber]->y-ey, 5);
	} else if (timer == 300) {
		for (int i = 0; i < 9; i++) {
			ex = rand()%250-75, ey = rand()%250-75;
			CreateExplosion(Enemies[BossNumber]->x-ex, Enemies[BossNumber]->y-ey, 5);
		}
		delete Enemies[BossNumber];
		Enemies[BossNumber] = nullptr;
		EnemyAlive[BossNumber] = false;
		Mix_PlayMusic( BGM2, -1 );
	} else if (timer > 300) {
		STATE_Pause = false;
		apply_surface(100, 75, VictoryScreen, Screen);
	}
}

void GameEntity::LoseGame(int timer) {
	if (timer == 1)
		Mix_PlayMusic( BGM3, -1 );
	apply_surface(100, 75, DefeatScreen, Screen);
}
//===============================================================================

void GameEntity::ProcessObject(GameObject *obj) {
	obj->Animation++;
	if (obj->Animation >= FxData[obj->TempFxNum]->AnimationMax) {
		obj->Animation = 0;
		if (obj->temporary)
			obj->Health--;
	}
	apply_surface((int)obj->x, (int)obj->y, Sprites[FxData[obj->TempFxNum]->SpriteSheet], Screen, FxData[obj->TempFxNum]->Clips[obj->Animation]);
}

void GameEntity::ProcessUnit(Unit *unit) {
	apply_surface((int)unit->x, (int)unit->y, Sprites[unit->Image], Screen, &CharClips[unit->ClipNum]);
}
void GameEntity::ProcessProjectile(Projectile *proj) {
	// move animation one frame forward; after end frame go back to first
	proj->Animation++;
	if (proj->Animation >= BulletData[proj->ProjType->BulletDataNumber]->AnimationMax)
		proj->Animation = 0;
	// fly forward
	if (proj->Fly()) {
		if (proj->Player) {
			for (int i = 0; i < 100; i++) {
				if (EnemyAlive[i]) {
					if (IntersectionProjectile(Enemies[i], proj)) {
						//printf("intersection = true");
						proj->ProjType->OnHit(i, proj->Id);
					}
				}
			}
		} else {
			if (Hero->Health > 0)
				if (IntersectionProjectile(Hero, proj)) {
					proj->ProjType->OnHit(0, proj->Id);
				}
		}
	}
	// draw at screen
	apply_surface((int)proj->x, (int)proj->y, Sprites[BulletData[proj->ProjType->BulletDataNumber]->SpriteSheet], Screen, BulletData[proj->ProjType->BulletDataNumber]->Clips[proj->Animation]);
}
//===============================================================================
void GameEntity::Shoot(Unit *unit) {
	if (unit->CurrentCooldown <= 0 && unit->Health > 0 && !Victory) {
		BulletCount++;
		if (BulletCount >= 30)
			BulletCount = 1;
		double ang = 0.0;
		if (unit->BulletType == BulletType[0]) {
			ang = 270.0;
			Bullet[BulletCount] = new Projectile(unit->BulletType, (int)unit->x+35, (int)unit->y, ang);
			Bullet[BulletCount]->Player = true;
			unit->CurrentCooldown = unit->Cooldown;
			Bullet[BulletCount]->Id = BulletCount;
			return;
		} else if (unit->BulletType == BulletType[1]) {
			ang = 90.0;
			Bullet[BulletCount] = new Projectile(unit->BulletType, (int)unit->x+16, (int)unit->y+55, ang);
			Bullet[BulletCount]->Player = false;
			unit->CurrentCooldown = unit->Cooldown;
			Bullet[BulletCount]->Id = BulletCount;
			return;
		} else if (unit->BulletType == BulletType[2]) {
			ang = 90.0;
			Bullet[BulletCount] = new Projectile(unit->BulletType, (int)unit->x+33, (int)unit->y+35, ang);
			Bullet[BulletCount]->Player = false;
			unit->CurrentCooldown = unit->Cooldown;
			Bullet[BulletCount]->Id = BulletCount;
			return;
		} else if (unit->BulletType == BulletType[3]) {
			ang = 90.0;
			Bullet[BulletCount] = new Projectile(unit->BulletType, (int)unit->x+36, (int)unit->y+110, ang);
			Bullet[BulletCount]->Player = false;
			unit->CurrentCooldown = unit->Cooldown;
			Bullet[BulletCount]->Id = BulletCount;

			BulletCount++;
			if (BulletCount >= 30)
				BulletCount = 1;
			Bullet[BulletCount] = new Projectile(unit->BulletType, (int)unit->x+76, (int)unit->y+110, ang);
			Bullet[BulletCount]->Player = false;
			unit->CurrentCooldown = unit->Cooldown;
			Bullet[BulletCount]->Id = BulletCount;
			return;
		} else if (unit->BulletType == BulletType[4]) {
			ang = 0.0;
			for (int i = 0; i < 5; i++) {
				ang+=30.0;
				BulletCount++;
				if (BulletCount >= 30)
					BulletCount = 1;
				Bullet[BulletCount] = new Projectile(unit->BulletType, (int)unit->x+40+i*5, (int)unit->y+110, ang);
				Bullet[BulletCount]->Player = false;
				unit->CurrentCooldown = unit->Cooldown;
				Bullet[BulletCount]->Id = BulletCount;
			}
			return;
		} else if (unit->BulletType == BulletType[5]) {
			ang = 270.0;
			Bullet[BulletCount] = new Projectile(unit->BulletType, (int)unit->x+35, (int)unit->y, ang);
			Bullet[BulletCount]->Player = true;
			unit->CurrentCooldown = unit->Cooldown+5;
			Bullet[BulletCount]->Id = BulletCount;
			return;
		}
	}
}

void GameEntity::CreateExplosion(int x, int y, int type) {
	FxCount++;
	if (FxCount >= 30)
		FxCount = 1;
	SpecialFx[FxCount] = new GameObject(0, x, y, 1);
	SpecialFx[FxCount]->temporary = true;
	SpecialFx[FxCount]->TempFxNum = type;
	if (type == 2)
		Mix_PlayChannel(-1, SFX[0], 0 );
	else if ((type == 3)||(type == 5))
		Mix_PlayChannel(-1, SFX[1], 0 );
	else if (type == 4)
		Mix_PlayChannel(-1, SFX[2], 0 );
}
//===============================================================================
// Processes the whole thing about keyboard controls
void GameEntity::HandleButtonPress(int key) {
	switch (key) {
		case BUTTON_SHOOT: 
			if (Hero->Health > 0)
				Shoot(Hero);
			break;
		case BUTTON_QUIT:
			EndGame = true;
			break;
		case BUTTON_ULTI1:
			if (Hero->Charge >= 6) {
				Hero->Charge-=6;
				Hero->BulletType = BulletType[5];
				Shoot(Hero);
				Hero->BulletType = BulletType[0];
			}
			//SDL_Thread *thread = SDL_CreateThread( Ultimate, nullptr );
			break;
		case BUTTON_ULTI2: 
			if (Hero->Charge >= 20 && !Victory && !Defeat) {
				Hero->Charge-=20;
				ExplosionCount = 0;
			}
			break;
		default:
			break;
    }
	if (Victory || Defeat) {
		if (STATE_Pause)
			return;
		char sym;
		if (key >= SDLK_0 && key <= SDLK_9 && PlayerName.length() <= 12) {
			if (!KEYHOLD_SHIFT)
				sym = 0x30+(key-SDLK_0);
			else
				sym = 0x20+(key-SDLK_0);
			PlayerName+=sym;
		} else if (key >= SDLK_a && key <= SDLK_z && PlayerName.length() <= 12) {
			if (!KEYHOLD_SHIFT)
				sym = 0x61+(key-SDLK_a);
			else
				sym = 0x41+(key-SDLK_a);
			PlayerName+=sym;
		} else if (key == SDLK_BACKSPACE) {
			PlayerName = PlayerName.substr(0, PlayerName.length() - 1);
		}
	}
}

void GameEntity::HandleButtonHold() {
	Uint8 *keystates = SDL_GetKeyState(NULL);
	bool left	= false, 
		 right	= false, 
		 up		= false, 
		 down	= false;

	if (keystates[SDLK_RSHIFT] || keystates[SDLK_LSHIFT])
		KEYHOLD_SHIFT = true;
	else
		KEYHOLD_SHIFT = false;
	if (Hero->Health < 0)
		return;
	if ((keystates[ BUTTON_RIGHT ])||(keystates[ SDLK_RIGHT])) {
		right = true;
		printf("right!\n");
	} else if ((keystates[ BUTTON_LEFT ])||(keystates[ SDLK_LEFT])) {
		printf("left!\n");
		left = true;
	}

	if ((keystates[ BUTTON_DOWN ])||(keystates[ SDLK_DOWN])) {
		printf("down!\n");
		down = true;
	} else if ((keystates[ BUTTON_UP ])||(keystates[ SDLK_UP])) {
		printf("up!\n");
		up = true;
	}

	Hero->DirectionalAccel(left, right, up, down);
}
//===============================================================================
GameEntity::GameEntity(SDL_Thread *thrd, SDL_Surface *scrn) {
	printf("Entity initialized!\n");
	Thread = thrd;
	Screen = scrn;
}

void GameEntity::StartGame() {
	Thread = SDL_CreateThread(InitGame, NULL);
}

bool GameEntity::PreloadImages() {
	// starry sky
	printf("Preloading images!");
	SkySquare = load_image("Data/Sky.png" );
	if ( SkySquare == NULL ) return false;
	// interface
	InterfaceOverlay = load_image(LanguageFolder+"/Sideboard.png");
	if ( InterfaceOverlay == NULL ) return false;
	HpBar = load_image("Data/LifebarFill.png");
	if ( HpBar == NULL ) return false;
	EnergyBar = load_image("Data/EnergybarFill.png");
	if ( EnergyBar == NULL ) return false;
	GrayBar = load_image("Data/BarGrayed.png");
	if ( GrayBar == NULL ) return false;
	VictoryScreen = load_image(LanguageFolder+"/Victory.png");
	if ( VictoryScreen == NULL ) return false;
	DefeatScreen = load_image(LanguageFolder+"/Defeat.png");
	if ( DefeatScreen == NULL ) return false;

	// main sprite sheet
	Sprites[0] = load_image( "Sprites/SpriteSheet.png" );
	if ( Sprites[0] == NULL ) return false;
	// main projectile sheet
	Sprites[1] = load_image( "Sprites/WeaponSheet.png" );
	if ( Sprites[1] == NULL ) return false;
	Sprites[2] = load_image( "Sprites/WeaponSheet2.png" );
	if ( Sprites[2] == NULL ) return false;
	Sprites[3] = load_image( "Sprites/WeaponSheet3.png" );
	if ( Sprites[3] == NULL ) return false;
	Sprites[4] = load_image( "Sprites/WeaponSheet4.png" );
	if ( Sprites[4] == NULL ) return false;
	Sprites[5] = load_image( "Sprites/WeaponSheet5.png" );
	if ( Sprites[5] == NULL ) return false;
	Sprites[11] = load_image( "Sprites/WeaponSheet6.png" );
	if ( Sprites[11] == NULL ) return false;
	// sheets with an explosion animations
	Sprites[6] = load_image( "Sprites/Explosion1.png" );
	if ( Sprites[6] == NULL ) return false;
	Sprites[7] = load_image( "Sprites/Explosion3.png" );
	if ( Sprites[7] == NULL ) return false;
	Sprites[8] = load_image( "Sprites/Explosion4.png" );
	if ( Sprites[8] == NULL ) return false;
	Sprites[9] = load_image( "Sprites/Explosion5.png" );
	if ( Sprites[9] == NULL ) return false;
	Sprites[10] = load_image( "Sprites/Explosion6.png" );
	if ( Sprites[10] == NULL ) return false;
	return true;
}

bool GameEntity::PreloadMusic() {
	BGM1 = NULL;
	BGM1 = Mix_LoadMUS("BGM/Main.ogg" ); 
	if (BGM1 == NULL) return false;

	BGM2 = NULL;
	BGM2 = Mix_LoadMUS("BGM/Fanfare.ogg" ); 
	if (BGM2 == NULL) return false;

	BGM3 = NULL;
	BGM3 = Mix_LoadMUS("BGM/Defeat.ogg" ); 
	if (BGM3 == NULL) return false;

	SFX[0] = NULL;
	SFX[0] = Mix_LoadWAV( "SFX/anvil.wav" );
	if( SFX[0] == NULL ) return false;

	SFX[1] = NULL;
	SFX[1] = Mix_LoadWAV( "SFX/bazooka.wav" );
	if( SFX[1] == NULL ) return false;

	SFX[2] = NULL;
	SFX[2] = Mix_LoadWAV( "SFX/trickshot.wav" );
	if( SFX[2] == NULL ) return false;

	return true;
}

bool GameEntity::SpriteClips() {
	InitClipValues();
	// set clip rectangles for unit sprites
	for (int i = 0; i <= Clip_Total; i++) {
		CharClips[i].x = Clip_ValuesA[i][0];
		CharClips[i].y = Clip_ValuesA[i][1];
		CharClips[i].w = Clip_ValuesB[i][0];
		CharClips[i].h = Clip_ValuesB[i][1];
	}

	// set clip rectangles for projectiles
	BulletData[0] = new SpriteData;
	BulletData[0]->SpriteSheet = 1;
	BulletData[0]->Init(20, 20, 1, 7, 7);

	BulletData[1] = new SpriteData;
	BulletData[1]->SpriteSheet = 2;
	BulletData[1]->Init(20, 20, 1, 5, 5);

	BulletData[2] = new SpriteData;
	BulletData[2]->SpriteSheet = 3;
	BulletData[2]->Init(15, 30, 1, 5, 5);

	BulletData[3] = new SpriteData;
	BulletData[3]->SpriteSheet = 4;
	BulletData[3]->Init(20, 20, 1, 5, 5);

	BulletData[4] = new SpriteData;
	BulletData[4]->SpriteSheet = 5;
	BulletData[4]->Init(20, 20, 1, 5, 5);

	BulletData[5] = new SpriteData;
	BulletData[5]->SpriteSheet = 11;
	BulletData[5]->Init(15, 30, 1, 5, 5);
	// set clip rectangles for explosions
	FxData[0] = new SpriteData;
	FxData[0]->SpriteSheet = 6;
	FxData[0]->Init(58, 62, 3, 3, 9);

	FxData[1] = new SpriteData;
	FxData[1]->SpriteSheet = 10;
	FxData[1]->Init(128, 128, 4, 4, 16);

	FxData[2] = new SpriteData;
	FxData[2]->SpriteSheet = 7;
	FxData[2]->Init(256, 256, 4, 4, 16);

	FxData[3] = new SpriteData;
	FxData[3]->SpriteSheet = 8;
	FxData[3]->Init(256, 256, 4, 4, 16);

	FxData[4] = new SpriteData;
	FxData[4]->SpriteSheet = 9;
	FxData[4]->Init(128, 128, 4, 4, 16);

	FxData[5] = new SpriteData;
	FxData[5]->SpriteSheet = 10;
	FxData[5]->Init(256, 256, 4, 4, 16);
	return true;
}

bool GameEntity::GenerateProjectileTypes() {
	int i = -1;

	i++; // 0 - hero basic
	BulletType[i] = new ProjectileType(1, 6.75, 0.4, 0, 1);
	BulletType[i]->GenerateCount = 1;
	BulletType[i]->AngleSeparate = 0.0;
	BulletType[i]->OnHit = &Exp01;

	i++; // 1 - type-A basic
	BulletType[i] = new ProjectileType(1, 5.25, 0.3, 1, 1);
	BulletType[i]->GenerateCount = 1;
	BulletType[i]->AngleSeparate = 0.0;
	BulletType[i]->OnHit = &Exp02;

	i++; // 2 - type-B basic
	BulletType[i] = new ProjectileType(1, 6.00, 0.4, 2, 1);
	BulletType[i]->GenerateCount = 1;
	BulletType[i]->AngleSeparate = 0.0;
	BulletType[i]->OnHit = &Exp02;

	i++; // 3 - boss basic
	BulletType[i] = new ProjectileType(2, 5.25, 0.5, 3, 1);
	BulletType[i]->GenerateCount = 2;
	BulletType[i]->AngleSeparate = 0.0;
	BulletType[i]->OnHit = &Exp03;

	i++; // 4 - boss advanced
	BulletType[i] = new ProjectileType(2, 3.25, 0.2, 4, 1);
	BulletType[i]->GenerateCount = 3;
	BulletType[i]->AngleSeparate = 60.0;
	BulletType[i]->OnHit = &Exp04;

	i++; // 5 - special attack 1 bullet
	BulletType[i] = new ProjectileType(3, 6.00, 0.4, 5, 1);
	BulletType[i]->GenerateCount = 1;
	BulletType[i]->AngleSeparate = 0.0;
	BulletType[i]->OnHit = &Exp05;
	return true;
}

bool GameEntity::CleanUp() {

	return true;
}

void GameEntity::GenerateOverlay() {
	// fill with grayed bars
	apply_surface(SCREEN_WIDTH+4, 191, GrayBar, Screen);
	apply_surface(SCREEN_WIDTH+4, 363, GrayBar, Screen);
	// clip the necessary amount of colored bars
	SDL_Rect *clipHp = new SDL_Rect(), *clipEnergy = new SDL_Rect();
	clipHp->x = 0; 
	clipHp->y = 0; 
	clipHp->h = 32; 
	clipEnergy->x = 0; 
	clipEnergy->y = 0; 
	clipEnergy->h = 32; 
	clipHp->w = Hero->Health*6+2;
	clipEnergy->w = Hero->Charge*6+2;
	// apply colored bars and interface
	apply_surface(SCREEN_WIDTH+4, 191, HpBar, Screen, clipHp);
	apply_surface(SCREEN_WIDTH+4, 363, EnergyBar, Screen, clipEnergy);
	apply_surface(SCREEN_WIDTH-5, 0, InterfaceOverlay, Screen);
	// draw score value
	TextStream.str(std::string());
	TextStream.clear();
	TextStream << Score;
	ScoreSurface = TTF_RenderText_Solid(FNT_Forte, TextStream.str().c_str(),  FNT_WhiteColor);
	apply_surface(SCREEN_WIDTH+55, 490, ScoreSurface, Screen );
	SDL_FreeSurface(ScoreSurface);
	// if victory or defeat, draws player name he was typing
	if (Victory || Defeat) {
		TextStream.str(std::string());
		TextStream.clear();
		TextStream << PlayerName;
		ScoreSurface = TTF_RenderText_Solid(FNT_Forte, TextStream.str().c_str(),  FNT_BlackColor);
		apply_surface(220, 375, ScoreSurface, Screen );
		SDL_FreeSurface(ScoreSurface);
	}
}

void GameEntity::ApplyBackground() {
	bgScroll+=2;
	if (bgScroll >= 512)
		bgScroll = 0;

	// middle part
	apply_surface(0, bgScroll, SkySquare, Screen);
	apply_surface(512, bgScroll, SkySquare, Screen);

	// upper part
	apply_surface(0, bgScroll-512, SkySquare, Screen);
	apply_surface(512, bgScroll-512, SkySquare, Screen);

	// lower part
	apply_surface(0, bgScroll+512, SkySquare, Screen);
	apply_surface(512, bgScroll+512, SkySquare, Screen);
}


void GameEntity::NewGame() {
	Mix_PlayMusic( BGM1, -1 );

	// set movement patterns
	PatternFunc[0] = &Pattern01;
	PatternFunc[1] = &Pattern02;
	PatternFunc[2] = &Pattern03;
	PatternFunc[3] = &Pattern04;
	PatternFunc[4] = &Pattern05;
	PatternFunc[5] = &Pattern06;
	PatternFunc[6] = &Pattern07;

	// create Hero
	Hero = new Unit(0, 0, 300, 340, HERO_HP_MAX, 0);
	Hero->Id = -1;
	Hero->IsEnemy = false;
	Hero->BulletType = BulletType[0];
	Hero->Charge = 0;
	Hero->CollisionIgnore = false;
	Hero->VelocityMax = 7.0;
	Hero->AccelX = 0.7;
	Hero->AccelY = 0.7;
	Hero->DecelRate = 2.0; 
	Hero->width-=4;

	// create enemy
	for (int i = 0; i < 100; i++)
		EnemyAlive[i] = false;
}

void GameEntity::AddEnemy1(int x, int y, int pattern) {
	EnemyCount++;
	if (EnemyCount > 99)
		EnemyCount = 0;
	Enemies[EnemyCount] = new Unit(0, 1, x, y, 3, 0);
	Enemies[EnemyCount]->Id = EnemyCount;
	Enemies[EnemyCount]->IsEnemy = true;
	Enemies[EnemyCount]->BulletType = BulletType[1];
	Enemies[EnemyCount]->VelocityMax = 2.5;
	Enemies[EnemyCount]->PatternNumber = pattern;
	Enemies[EnemyCount]->CollisionIgnore = true;
	EnemyAlive[EnemyCount] = true;
}

void GameEntity::AddEnemy2(int x, int y, int pattern) {
	EnemyCount++;
	if (EnemyCount > 99)
		EnemyCount = 0;
	Enemies[EnemyCount] = new Unit(0, 2, x, y, 1, 0);
	Enemies[EnemyCount]->Id = EnemyCount;
	Enemies[EnemyCount]->IsEnemy = true;
	Enemies[EnemyCount]->BulletType = BulletType[2];
	Enemies[EnemyCount]->VelocityMax = 4.5;
	Enemies[EnemyCount]->PatternNumber = pattern;
	Enemies[EnemyCount]->CollisionIgnore = true;
	EnemyAlive[EnemyCount] = true;
}

void GameEntity::AddEnemy3(int x, int y, int pattern) {
	EnemyCount++;
	if (EnemyCount > 99)
		EnemyCount = 0;
	Enemies[EnemyCount] = new Unit(0, 3, x, y, 100, 0);
	Enemies[EnemyCount]->Id = EnemyCount;
	Enemies[EnemyCount]->IsEnemy = true;
	Enemies[EnemyCount]->BulletType = BulletType[3];
	Enemies[EnemyCount]->VelocityMax = 1.85;
	Enemies[EnemyCount]->PatternNumber = pattern;
	Enemies[EnemyCount]->CollisionIgnore = true;
	EnemyAlive[EnemyCount] = true;
	BossNumber = EnemyCount;
}


void GameEntity::EnemySpawn(int timer) {
	if (timer%30 == 0) {
		if (Hero->Charge < 20)
			Hero->Charge++;
	}
	// first wave - 4 type-A
	if (timer == 100) { 
		AddEnemy1(0, 0, 0);
		AddEnemy1(640, 100, 1);
		return;
	}
	if (timer == 250) {
		AddEnemy1(0, 150, 0);
		AddEnemy1(640, 250, 1);
		return;
	}
	
	// second wave - 4 pairs of type-B
	if (timer == 400)
		AddEnemy2(0, 0, 4);
	if (timer == 410)
		AddEnemy2(0, 0, 4);
	if (timer == 500)
		AddEnemy2(0, 0, 4);
	if (timer == 510)
		AddEnemy2(0, 0, 4);
	if (timer == 580)
		AddEnemy2(640, 0, 5);
	if (timer == 590)
		AddEnemy2(640, 0, 5);
	if (timer == 650)
		AddEnemy2(640, 0, 5);
	if (timer == 660)
		AddEnemy2(640, 0, 5);

	// third wave - 3 times 4 of type-A
	if (timer == 750) {
		AddEnemy1(0, 0, 0);
		AddEnemy1(0, 300, 3);
		AddEnemy1(640, 0, 1);
		AddEnemy1(640, 300, 2);
	}
	if (timer == 850) {
		AddEnemy1(0, 50, 0);
		AddEnemy1(0, 250, 3);
		AddEnemy1(640, 50, 1);
		AddEnemy1(640, 250, 2);
	}
	if (timer == 950) {
		AddEnemy1(0, 0, 0);
		AddEnemy1(0, 300, 3);
		AddEnemy1(640, 0, 1);
		AddEnemy1(640, 300, 2);
	}

	// fourth wave - wave of 8 type-A covered by 8 type-B
	if (timer == 1050) {
		AddEnemy1(0, 0, 0);
		AddEnemy1(0, 200, 3);
	}
	if (timer == 1080) {
		AddEnemy1(0, 5, 0);
		AddEnemy1(0, 250, 3);
	}
	if (timer == 1110) {
		AddEnemy1(640, 0, 1);
		AddEnemy1(640, 300, 2);
	}
	if (timer == 1140) {
		AddEnemy1(640, 50, 1);
		AddEnemy1(640, 250, 2);
	}
	if (timer == 1160)
		AddEnemy2(0, 0, 4);
	if (timer == 1180)
		AddEnemy2(640, 0, 5);
	if (timer == 1200)
		AddEnemy2(0, 30, 4);
	if (timer == 1220)
		AddEnemy2(640, 30, 5);
	if (timer == 1240)
		AddEnemy2(0, 60, 4);
	if (timer == 1260)
		AddEnemy2(640, 60, 5);
	if (timer == 1280)
		AddEnemy2(0, 90, 4);
	if (timer == 1300)
		AddEnemy2(640, 90, 5);

	// fifth wave - mix of type-A and type-B
	if (timer == 1400) {
		AddEnemy1(0, 200, 0);
		AddEnemy1(640, 0, 1);
	}
	if (timer == 1440) {
		AddEnemy1(0, 200, 0);
		AddEnemy1(640, 0, 1);
	}
	if (timer == 1450)
		AddEnemy2(0, 0, 3);
	if (timer == 1460)
		AddEnemy2(640, 0, 2);
	if (timer == 1470)
		AddEnemy2(0, 90, 4);
	if (timer == 1480)
		AddEnemy2(640, 90, 5);
	if (timer == 1500) {
		AddEnemy1(0, 200, 0);
		AddEnemy1(640, 0, 1);
	}
	if (timer == 1540) {
		AddEnemy1(0, 200, 0);
		AddEnemy1(640, 0, 1);
	}
	if (timer == 1560)
		AddEnemy2(0, 0, 3);
	if (timer == 1570)
		AddEnemy2(640, 0, 2);
	if (timer == 1580)
		AddEnemy2(0, 90, 4);
	if (timer == 1590)
		AddEnemy2(640, 90, 5);
	if (timer == 1640) {
		AddEnemy1(0, 200, 0);
		AddEnemy1(640, 0, 1);
		AddEnemy1(0, 200, 0);
		AddEnemy1(640, 0, 1);
	}
	if (timer == 1700) {
		AddEnemy1(0, 200, 0);
		AddEnemy1(640, 0, 1);
		AddEnemy1(0, 200, 0);
		AddEnemy1(640, 0, 1);
	}

	// boss
	if (timer == 1820)
		AddEnemy3(300, -100, 6);
	// boss adds
	if (timer == 1900) {
		AddEnemy1(0, 200, 0);
		AddEnemy1(640, 0, 1);
	}
	if (timer == 1980)
		AddEnemy2(0, 0, 3);
	if (timer == 2010)
		AddEnemy2(640, 0, 2);
	if (timer == 2050) {
		AddEnemy1(0, 200, 0);
		AddEnemy1(640, 0, 1);
	}
	if (timer == 2110)
		AddEnemy2(0, 90, 4);
	if (timer == 2140)
		AddEnemy2(640, 90, 5);
	if (timer == 2170)
		AddEnemy2(0, 0, 3);
	if (timer == 2200)
		AddEnemy2(640, 0, 2);
	if (timer == 2230) {
		AddEnemy1(0, 200, 0);
		AddEnemy1(640, 0, 1);
	}
	if (timer == 2300) {
		AddEnemy1(0, 200, 0);
		AddEnemy1(640, 0, 1);
	}
	if (timer == 2400)
		AddEnemy2(0, 90, 4);
	if (timer == 2430)
		AddEnemy2(640, 90, 5);
	if (timer == 2460)
		AddEnemy2(0, 0, 3);
	if (timer == 2490)
		AddEnemy2(640, 0, 2);
	if (timer == 2520)
		AddEnemy2(0, 90, 4);
	if (timer == 2550)
		AddEnemy2(640, 90, 5);
	if (timer == 2580)
		AddEnemy2(0, 0, 3);
	if (timer == 2610)
		AddEnemy2(640, 0, 2);
}


int GameEntity::InitGame(void *data) {
	printf("Init started!\n");
	EndGame = false;
	FNT_Forte = TTF_OpenFont("Data/FORTE.TTF", 34);

	// stop menu music
	Mix_HaltMusic(); 

	// preload loading picture
	LoadScreenBG = NULL;
	LoadScreenBG = load_image("Data/LoadingBG.png" );
	if ( LoadScreenBG == NULL ) return 3;

	// display it
	apply_surface(0, 0, LoadScreenBG, Screen);
	if( SDL_Flip(Screen) == -1 ) return 0;

	// start loading surfaces
	if (!PreloadImages()) {
		printf("Preloading images failed!\n");
	}
	// loads sounds and music
	if (!PreloadMusic()) {
		printf("Preloading music failed!\n");
	}
	// load character clips from sprite sheets
	if (!SpriteClips()) {
		printf("Clipping sprites failed!\n");
	}
	// load character clips from sprite sheets
	if (!GenerateProjectileTypes()) {
		printf("Generating projectile types failed!\n");
	}
	// delay after loading (optional)
	SDL_Delay(1000);

	// create background
	bgScroll = 0;
	SDL_FillRect(Screen, &Screen->clip_rect, SDL_MapRGB(Screen->format, 0x00, 0x00, 0x00));
	if( SDL_Flip(Screen) == -1 ) return 0;
	
	ApplyBackground();
	if( SDL_Flip(Screen) == -1 ) return 1;

	int FPScount = 0; 
	bool FPScap = true; 
	Timer fps;

	NewGame();
	
	while(!EndGame) {
		fps.start();
		ApplyBackground();
		// --- hero
		
		HandleButtonHold();
		if (!Victory && !Defeat)
			Hero->Move();

		if (Hero->CurrentCooldown > 0)
			Hero->CurrentCooldown--;
		if (Hero->Health > 0)
			ProcessUnit(Hero);
		if (ExplosionCount > -1) {
			ExplosionCount++;
			ExplosionWave(ExplosionCount);
		}
		// --- enemy units
		SpawnTimer++;
		for (int i = 0; i < 100; i++) {
			if (EnemyAlive[i]) {
				if (Enemies[i]->Health > 0) {
					ProcessUnit(Enemies[i]);
					if (!Victory && !Defeat) {
						Enemies[i]->Move();
						PatternFunc[Enemies[i]->PatternNumber](Enemies[i]);
					}
				} else {
					delete Enemies[i];
					Enemies[i] = nullptr;
					EnemyAlive[i] = false;
				}
				
			}
		}
		// --- projectiles
		for (int i = 1; i <= 30; i++) {
			if (Bullet[i] != nullptr) {
				if (Bullet[i]->Health > 0) {
					ProcessProjectile(Bullet[i]);
				} else {
					delete Bullet[i];
					Bullet[i] = nullptr;
				}
			}
		}

		// --- special fx
		for (int i = 1; i <= 29; i++) {
			if (SpecialFx[i] != nullptr) {
				if (SpecialFx[i]->Health > 0) {
					ProcessObject(SpecialFx[i]);
				} else {
					delete SpecialFx[i];
					SpecialFx[i] = nullptr;
				}
			}
		}
		// victory/defeat/enemyspawn
		if (Victory) {
			WinGame(SpawnTimer);
		} else if (Defeat) {
			LoseGame(SpawnTimer);
		} else {
			EnemySpawn(SpawnTimer);
		}

		// --- Overlay
		GenerateOverlay();
		// --- Update the screen
		if( SDL_Flip(Screen) == -1 ) return 1;
	
		FPScount++; // increment frame counter
		if( (FPScap) && ( fps.get_ticks() < 1000/FRAMES_PER_SECOND ) ) {
			// Sleep the remaining frame time 
			SDL_Delay( (1000/FRAMES_PER_SECOND) - fps.get_ticks() );
		}
	}
	CleanUp();
	return 0;
}