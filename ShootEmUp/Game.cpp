#include "Game.h"

// surfaces
SDL_Surface *LoadScreenBG;
SDL_Surface *SkySquare;
SDL_Surface *Sprites[100];

// Music
Mix_Music *BGM1 = NULL;

// gameplay
GameEntity *GE;
Unit *hero;
SDL_Rect CharClips[100];

//===============================================================================
GameObject::GameObject() {
	Image = 0;
	x = 0;
	y = 0;
	Health = 1;
	ClipNum = 0;
}

GameObject::GameObject(int DataImageNumber, int PosX, int PosY, int hp) {
	Image = DataImageNumber;
	x = PosX;
	y = PosY;
	Health = hp;
	ClipNum = 0;
}

void GameObject::Refresh() {
	int xt;
	apply_surface(x, y, Sprites[Image], GE->Screen, &CharClips[ClipNum]);
}

//===============================================================================
Unit::Unit(int DataImageNumber, int PosX, int PosY, int hp, int BltType) {
	x = PosX;
	y = PosY;
	Health = hp;
	Image = DataImageNumber;
}

//===============================================================================
void HandleInput() {
/*	
	Uint8 *keystates = SDL_GetKeyState(NULL);
	
	}
*/
}
//===============================================================================
GameEntity::GameEntity(SDL_Thread *thrd, SDL_Surface *scrn) {
	printf("Entity initialized!\n");
	Thread = thrd;
	ThreadFnc = InitGame;
	Screen = scrn;
}

void GameEntity::StartGame() {
	Thread = SDL_CreateThread(ThreadFnc, this);
}

bool PreloadImages() {
	// starry sky
	SkySquare = NULL;
	SkySquare = load_image("Data/Sky.png" );
	if ( SkySquare == NULL ) return false;
	// main sprite sheet
	Sprites[0] = load_image( "Sprites/SpriteSheet.png" );
	if ( Sprites[0] == NULL ) return false;

	return true;
}

bool PreloadMusic() {
	BGM1 = Mix_LoadMUS("BGM/Main.ogg" ); 
	if (BGM1 == NULL) return false;
}

bool SpriteClips() {
	CharClips[0].x = 0;
	CharClips[0].y = 0;
	CharClips[0].w = 90;
	CharClips[0].h = 90;
	return true;
}

bool CleanUp() {

	return true;
}

void ApplyBackground() {
	GE->bgScroll+=2;
	if (GE->bgScroll >= 512)
		GE->bgScroll = 0;

	// middle part
	apply_surface(0, GE->bgScroll, SkySquare, GE->Screen);
	apply_surface(512, GE->bgScroll, SkySquare, GE->Screen);

	// upper part
	apply_surface(0, GE->bgScroll-512, SkySquare, GE->Screen);
	apply_surface(512, GE->bgScroll-512, SkySquare, GE->Screen);

	// lower part
	apply_surface(0, GE->bgScroll+512, SkySquare, GE->Screen);
	apply_surface(512, GE->bgScroll+512, SkySquare, GE->Screen);
}


void NewGame() {
	Mix_VolumeMusic(50);
	Mix_PlayMusic( BGM1, -1 );

	hero = new Unit(0, 300, 340, 5, 0);
	hero->Id = 0;
	hero->ClipNum = 0;
}


int InitGame(void *data) {
	printf("Init started!\n");
	GE = (GameEntity*)data;
	GE->EndGame = false;

	// stop menu music
	Mix_HaltMusic(); 

	// preload loading picture
	LoadScreenBG = NULL;
	LoadScreenBG = load_image("Data/LoadingBG.png" );
	if ( LoadScreenBG == NULL ) return 3;

	// display it
	apply_surface(0, 0, LoadScreenBG, GE->Screen);
	if( SDL_Flip( GE->Screen ) == -1 ) return 0;

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
	GE->bgScroll = 0;
	SDL_FillRect(GE->Screen, &GE->Screen->clip_rect, SDL_MapRGB( GE->Screen->format, 0x00, 0x00, 0x00 ) );
	if( SDL_Flip( GE->Screen ) == -1 ) return 0;
	
	ApplyBackground();
	if( SDL_Flip( GE->Screen ) == -1 ) return 1;

	int FPScount = 0; 
	bool FPScap = true; 
	Timer fps;

	NewGame();
	
	while(!GE->EndGame) {
		fps.start();
		ApplyBackground();

		hero->Refresh();
		//hero->Move();

		//Update the screen
		if( SDL_Flip( GE->Screen ) == -1 ) return 1;
	
		FPScount++; //increment frame counter
		if( (FPScap) && ( fps.get_ticks() < 1000/FRAMES_PER_SECOND ) ) {
			//Sleep the remaining frame time 
			SDL_Delay( (1000/FRAMES_PER_SECOND) - fps.get_ticks() );
		}
	}
	CleanUp();
	return 0;
}