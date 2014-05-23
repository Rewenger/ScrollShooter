#include "Game.h"

//unsigned char test::X = 1;
bool GameEntity::EndGame = false;
int GameEntity::bgScroll = 0;
int GameEntity::BulletCount = 0;
int GameEntity::FxCount = 0;
SDL_Thread *GameEntity::Thread = NULL;
SDL_Surface *GameEntity::Screen = NULL;
SDL_Surface *GameEntity::LoadScreenBG = NULL;
SDL_Surface *GameEntity::SkySquare = NULL;
SDL_Surface *GameEntity::InterfaceOverlay = NULL;
SDL_Surface *GameEntity::Sprites[100];
SDL_Surface *GameEntity::HpBar;
SDL_Surface *GameEntity::EnergyBar;
SDL_Surface *GameEntity::GrayBar;

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
Mix_Chunk *GameEntity::SFX[100];
//===============================================================================
// check whether unit intersects with another unit
bool GameEntity::IntersectionUnits(Unit *unit1, Unit *unit2) {
	return (unit1->x <= unit2->x+unit2->width &&
          unit2->x <= unit1->x+unit1->width &&
          unit1->y <= unit2->y+unit2->height &&
          unit2->y <= unit1->y+unit1->height);
}

// check whether unit intersects with projectile
bool GameEntity::IntersectionProjectile(Unit *unit, Projectile *proj) {
		return (unit->x <= proj->x+proj->width &&
          proj->x <= unit->x+unit->width &&
          unit->y <= proj->y+proj->width &&
          proj->y <= unit->y+unit->height);
}
// explosion of bullet #1
void GameEntity::Exp01(int target, int bullet) {
	Enemies[target]->Damage(Bullet[bullet]->ProjType->Damage);
	if (Enemies[target]->Health <= 0) {
		CreateExplosion(Enemies[target]->x-100, Enemies[target]->y-100, 3);
	}
	Bullet[bullet]->Health--;
	CreateExplosion(Bullet[bullet]->x-100, Bullet[bullet]->y-100, 2);
}


// movement pattern #1
// moves diagonally downright, follows right, diagonally right-upwards and disappears
void Pattern01(Unit *target) {
	target->PatternFrameCount++;
	int x = target->PatternFrameCount;
	if (x < 30) {
		target->DirectionalAccel(false, true, false, true);
	}
	if ((x > 30)&&(x < 200)) {
		target->DirectionalAccel(false, true, false, false);
	}
	if ((x > 200)&&(x < 230)) {
		target->DirectionalAccel(false, true, true, false);
	}
	if (x > 230) {
		target->Health = 0;
	}
}
// movement pattern #2
// moves diagonally downleft, follows left, diagonally left-upwards and disappears
void Pattern02(Unit *target) {
	target->PatternFrameCount++;
	int x = target->PatternFrameCount;
	if (x < 30) {
		target->DirectionalAccel(true, false, false, true);
	}
	if ((x > 30)&&(x < 200)) {
		target->DirectionalAccel(true, false, false, false);
	}
	if ((x > 200)&&(x < 230)) {
		target->DirectionalAccel(true, false, true, false);
	}
	if (x > 230) {
		target->Health = 0;
	}
}
// movement pattern #3
// moves horizontally left and disappears
void Pattern03(Unit *target) {
	target->PatternFrameCount++;
	int x = target->PatternFrameCount;
	if (x < 160) {
		target->DirectionalAccel(true, false, false, false);
	}
	if (x > 160) {
		target->Health = 0;
	}
}
// movement pattern #4
// moves horizontally right and disappears
void Pattern04(Unit *target) {
	target->PatternFrameCount++;
	int x = target->PatternFrameCount;
	if (x < 160) {
		target->DirectionalAccel(false, true, false, false);
	}
	if (x > 160) {
		target->Health = 0;
	}
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
						printf("intersection = true");
						proj->ProjType->OnHit(i, proj->Id);
					}
				}
			}
		} else {
			if (IntersectionProjectile(Hero, proj)) {
				proj->ProjType->OnHit(-1, proj->Id);
			}
		}
	}
	// draw at screen
	apply_surface((int)proj->x, (int)proj->y, Sprites[BulletData[proj->ProjType->BulletDataNumber]->SpriteSheet], Screen, BulletData[proj->ProjType->BulletDataNumber]->Clips[proj->Animation]);
}
//===============================================================================
void GameEntity::Shoot(Unit *unit) {
	if (unit->CurrentCooldown <= 0) {
		BulletCount++;
		if (BulletCount >= 30)
			BulletCount = 1;
		void (*trigger)(int TargetId, int ProjectileId);
		double ang = 0.0;
		if (unit->BulletType == BulletType[0]) {
			trigger = &Exp01;
			ang = 270.0;
			Bullet[BulletCount] = new Projectile(unit->BulletType, (int)unit->x+35, (int)unit->y, ang);
			Bullet[BulletCount]->Player = true;
			unit->CurrentCooldown = unit->Cooldown;
			Bullet[BulletCount]->Id = BulletCount;
			//if (Sprites[shtcnt*5+animation] != nullptr) 
			//SetAnimation(1);
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
	if (type == 3)
		Mix_PlayChannel(-1, SFX[1], 0 );
}
//===============================================================================
// Processes the whole thing about keyboard controls
void GameEntity::HandleButtonPress(int key) {
	switch (key) {
		case BUTTON_SHOOT: 
			Hero->Charge--;
			if (Hero->Health > 0)
				Shoot(Hero);
			break;
		case BUTTON_QUIT:
			EndGame = true;
			break;
		case BUTTON_ULTI: 
			//SDL_Thread *thread = SDL_CreateThread( Ultimate, nullptr );
			break;
    }

}

void GameEntity::HandleButtonHold() {
	Uint8 *keystates = SDL_GetKeyState(NULL);
	bool left	= false, 
		 right	= false, 
		 up		= false, 
		 down	= false;

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
	// sheets with an explosion animations
	Sprites[4] = load_image( "Sprites/Explosion1.png" );
	if ( Sprites[4] == NULL ) return false;
	Sprites[5] = load_image( "Sprites/Explosion2.png" );
	if ( Sprites[5] == NULL ) return false;
	Sprites[6] = load_image( "Sprites/Explosion3.png" );
	if ( Sprites[6] == NULL ) return false;
	Sprites[7] = load_image( "Sprites/Explosion4.png" );
	if ( Sprites[7] == NULL ) return false;
	return true;
}

bool GameEntity::PreloadMusic() {
	BGM1 = NULL;
	BGM1 = Mix_LoadMUS("BGM/Main.ogg" ); 
	if (BGM1 == NULL) return false;

	SFX[0] = NULL;
	SFX[0] = Mix_LoadWAV( "SFX/anvil.wav" );
	if( SFX[0] == NULL ) return false;

	SFX[1] = NULL;
	SFX[1] = Mix_LoadWAV( "SFX/bazooka.wav" );
	if( SFX[1] == NULL ) return false;

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
	BulletData[1]->Init(15, 30, 1, 5, 5);

	BulletData[2] = new SpriteData;
	BulletData[2]->SpriteSheet = 3;
	BulletData[2]->Init(15, 30, 1, 5, 5);
	// set clip rectangles for explosions
	FxData[0] = new SpriteData;
	FxData[0]->SpriteSheet = 4;
	FxData[0]->Init(58, 62, 3, 3, 9);

	FxData[1] = new SpriteData; // not working
	FxData[1]->SpriteSheet = 5;
	FxData[1]->Init(90, 96, 4, 5, 15);

	FxData[2] = new SpriteData;
	FxData[2]->SpriteSheet = 6;
	FxData[2]->Init(256, 256, 4, 4, 16);

	FxData[3] = new SpriteData;
	FxData[3]->SpriteSheet = 7;
	FxData[3]->Init(256, 256, 4, 4, 16);
	return true;
}

bool GameEntity::GenerateProjectileTypes() {
	int i = -1;

	i++;
	BulletType[i] = new ProjectileType(1, 6.75, 0.4, 0, 1);
	BulletType[i]->GenerateCount = 1;
	BulletType[i]->AngleSeparate = 0.0;
	BulletType[i]->OnHit = &Exp01;

	i++;
	BulletType[i] = new ProjectileType(1, 5.25, 0.3, 1, 1);
	BulletType[i]->GenerateCount = 1;
	BulletType[i]->AngleSeparate = 0.0;
	BulletType[i]->OnHit = &Exp01;

	i++;
	BulletType[i] = new ProjectileType(1, 6.00, 0.4, 2, 1);
	BulletType[i]->GenerateCount = 1;
	BulletType[i]->AngleSeparate = 0.0;
	BulletType[i]->OnHit = &Exp01;
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

	// create Hero
	Hero = new Unit(0, 0, 300, 340, HERO_HP_MAX, 0);
	Hero->Id = -1;
	Hero->IsEnemy = false;
	Hero->BulletType = BulletType[0];
	Hero->Charge = 10;

	// create enemy
	for (int i = 0; i < 100; i++)
		EnemyAlive[i] = false;

	Enemies[0] = new Unit(0, 1, 0, 0, 4, 0);
	Enemies[0]->Id = 0;
	Enemies[0]->IsEnemy = true;
	Enemies[0]->BulletType = BulletType[1];
	Enemies[0]->VelocityMax = 2.5;
	Enemies[0]->PatternNumber = 0;
	EnemyAlive[0] = true;

	Enemies[1] = new Unit(0, 2, 640, 135, 4, 0);
	Enemies[1]->Id = 1;
	Enemies[1]->IsEnemy = true;
	Enemies[1]->BulletType = BulletType[1];
	Enemies[1]->VelocityMax = 2.5;
	Enemies[1]->PatternNumber = 1;
	EnemyAlive[1] = true;
}


int GameEntity::InitGame(void *data) {
	printf("Init started!\n");
	EndGame = false;

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
		if (Hero->Health > 0)
			HandleButtonHold();

		Hero->Move();

		if (Hero->CurrentCooldown > 0)
			Hero->CurrentCooldown--;
		if (Hero->Health > 0)
			ProcessUnit(Hero);
		// --- enemy units

		for (int i = 0; i < 100; i++) {
			if (EnemyAlive[i]) {
				if (Enemies[i]->Health > 0) {
					ProcessUnit(Enemies[i]);
					Enemies[i]->Move();
					PatternFunc[Enemies[i]->PatternNumber](Enemies[i]);
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