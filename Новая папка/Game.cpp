#include "Game.h"
#include "GameObjects.h"

//===============================================================================


//===============================================================================
void GameEntity::HandleInput() {
/*	
	Uint8 *keystates = SDL_GetKeyState(NULL);
	
	}
*/
}
//===============================================================================
GameEntity::GameEntity(SDL_Thread *thrd, SDL_Surface *scrn) {
	printf("Entity initialized!\n");
	Thread = thrd;
	Screen = scrn;
}

void GameEntity::StartGame() {
	Thread = SDL_CreateThread(InitGame, this);
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
}

bool GameEntity::SpriteClips() {
	CharClips[0].x = 0;
	CharClips[0].y = 0;
	CharClips[0].w = 90;
	CharClips[0].h = 90;
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

	Hero = new Unit(0, 300, 340, 5, 0, thislink);
	Hero->Id = 0;
	Hero->ClipNum = 0;
}


int GameEntity::InitGame(void *data) {
	printf("Init started!\n");
	thislink = data;
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

		Hero->Refresh();
		//hero->Move();

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