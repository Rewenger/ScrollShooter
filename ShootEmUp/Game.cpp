#include "Game.h"

//unsigned char test::X = 1;
bool GameEntity::EndGame = false;
int GameEntity::bgScroll = 0;
SDL_Thread *GameEntity::Thread = NULL;
SDL_Surface *GameEntity::Screen = NULL;
SDL_Surface *GameEntity::LoadScreenBG = NULL;
SDL_Surface *GameEntity::SkySquare = NULL;
SDL_Surface *GameEntity::Sprites[100];
Unit *GameEntity::Hero;
SDL_Rect GameEntity::CharClips[100];
Mix_Music *GameEntity::BGM1;

//===============================================================================
void GameEntity::ProcessObject(GameObject *obj) {
	apply_surface(obj->x, obj->y, Sprites[obj->Image], Screen, &CharClips[obj->ClipNum]);
}

void GameEntity::ProcessUnit(Unit *unit) {
	apply_surface(unit->x, unit->y, Sprites[unit->Image], Screen, &CharClips[unit->ClipNum]);
}
void GameEntity::ProcessProjectile(Projectile *proj) {

}
//===============================================================================
// Processes the whole thing about keyboard controls
void GameEntity::HandleButtonPress(int key) {
	switch (key) {
		case BUTTON_SHOOT: 
			//Hero->Shoot(); 
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
		HandleButtonHold();

		Hero->Move();
		ProcessUnit(Hero);

		//Update the screen
		if( SDL_Flip(Screen) == -1 ) return 1;
	
		FPScount++; //increment frame counter
		if( (FPScap) && ( fps.get_ticks() < 1000/FRAMES_PER_SECOND ) ) {
			//Sleep the remaining frame time 
			SDL_Delay( (1000/FRAMES_PER_SECOND) - fps.get_ticks() );
		}
	}
	CleanUp();
	return 0;
}