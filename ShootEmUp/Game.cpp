#include "Game.h"

//unsigned char test::X = 1;
bool GameEntity::EndGame = false;
int GameEntity::bgScroll = 0;
int GameEntity::BulletCount = 0;
SDL_Thread *GameEntity::Thread = NULL;
SDL_Surface *GameEntity::Screen = NULL;
SDL_Surface *GameEntity::LoadScreenBG = NULL;
SDL_Surface *GameEntity::SkySquare = NULL;
SDL_Surface *GameEntity::Sprites[100];

Unit *GameEntity::Hero;
Projectile *GameEntity::Bullet[100];
ProjectileType *GameEntity::BulletType[10];

SDL_Rect GameEntity::CharClips[100];
SDL_Rect GameEntity::BulletClips[100];
Mix_Music *GameEntity::BGM1;

//===============================================================================
void GameEntity::Shoot(Unit *unit) {
	if (unit->CurrentCooldown <= 0) {
		BulletCount++;
		if (BulletCount > 30)
			BulletCount = 1;
		void (*trigger)(int TargetId, int ProjectileId);
		double ang = 0.0;
		if (unit->BulletType == BulletType[0]) {
			trigger = &Exp01;
			ang = 270.0;
			Bullet[BulletCount] = new Projectile(unit->BulletType, unit->x+35, unit->y, ang);
			Bullet[BulletCount]->Player = true;
			unit->CurrentCooldown = unit->Cooldown;
			Bullet[BulletCount]->Id = BulletCount;
			//if (Sprites[shtcnt*5+animation] != nullptr) 
			//SetAnimation(1);
		}
	}
}

void GameEntity::ProcessObject(GameObject *obj) {
	apply_surface(obj->x, obj->y, Sprites[obj->Image], Screen, &CharClips[obj->ClipNum]);
}

void GameEntity::ProcessUnit(Unit *unit) {
	apply_surface(unit->x, unit->y, Sprites[unit->Image], Screen, &CharClips[unit->ClipNum]);
}
void GameEntity::ProcessProjectile(Projectile *proj) {
	// move animation one frame forward; after end frame go back to first
	proj->Animation++;
	if (proj->Animation > proj->AnimationMax)
		proj->Animation = 0;
	// fly forward
	if (proj->Fly()) {

	}
	// draw at screen
	apply_surface(proj->x, proj->y, Sprites[1], Screen, &BulletClips[proj->ProjType->SpriteStart+proj->Animation]);
}
//===============================================================================
// Processes the whole thing about keyboard controls
void GameEntity::HandleButtonPress(int key) {
	switch (key) {
		case BUTTON_SHOOT: 
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

void HandleButtonHold();
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
	SkySquare = NULL;
	SkySquare = load_image("Data/Sky.png" );
	if ( SkySquare == NULL ) return false;
	// main sprite sheet
	Sprites[0] = load_image( "Sprites/SpriteSheet.png" );
	if ( Sprites[0] == NULL ) return false;
	// main projectile sheet
	Sprites[1] = load_image( "Sprites/WeaponSheet.png" );
	if ( Sprites[1] == NULL ) return false;
	return true;
}

bool GameEntity::PreloadMusic() {
	BGM1 = NULL;
	BGM1 = Mix_LoadMUS("BGM/Main.ogg" ); 
	if (BGM1 == NULL) return false;
	return true;
}

bool GameEntity::SpriteClips() {
	InitClipValues();

	for (int i = 0; i <= Clip_Total; i++) {
		CharClips[i].x = Clip_ValuesA[i][0];
		CharClips[i].y = Clip_ValuesA[i][1];
		CharClips[i].w = Clip_ValuesB[i][0];
		CharClips[i].h = Clip_ValuesB[i][1];
	}

	int shift = -20;
	for (int i = 0; i <= 7; i++) {
		shift+=20;
		BulletClips[i].x = shift;
		BulletClips[i].y = 0;
		BulletClips[i].w = 20;
		BulletClips[i].h = 20;
	}
	return true;
}

void GameEntity::Exp01(int target, int bullet) {
	/*Enemies[en]->Damage(temp->dmg);
	FXcount++;
	if (FXcount > 10) FXcount = 1;
    Effects[FXcount] = new TObject(10, temp->x, temp->y, 1, 10, 4);
	Effects[FXcount]->temporary = true;
	FAvailable[FXcount] = true;
	temp->Kill();*/
}

bool GameEntity::GenerateProjectileTypes() {
	int i = -1;

	i++;
	BulletType[i] = new ProjectileType(1, 6.25, 0.4, 0, 7, 1);
	BulletType[i]->GenerateCount = 1;
	BulletType[i]->AngleSeparate = 0.0;
	return true;
}

bool GameEntity::CleanUp() {

	return true;
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
	Mix_VolumeMusic(50);
	Mix_PlayMusic( BGM1, -1 );

	Hero = new Unit(0, 0, 300, 340, 5, 0);
	Hero->Id = 0;
	Hero->IsEnemy = false;
	Hero->BulletType = BulletType[0];
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
		ProcessUnit(Hero);
		// --- enemy units

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