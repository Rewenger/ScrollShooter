#include "Game.h"

SDL_Surface *LoadScreenBG;
SDL_Surface *SkySquare;
GameEntity *GE;

GameEntity::GameEntity(SDL_Thread *thrd, SDL_Surface *scrn) {
	printf("Entity initialized!\n");
	Thread = thrd;
	ThreadFnc = InitGame;
	Screen = scrn;
}

void GameEntity::StartGame() {
	Thread = SDL_CreateThread(ThreadFnc, this);
}

bool PreloadData() {
	// load
	SkySquare = NULL;
	SkySquare = load_image("Data/Sky.png" );
	if ( SkySquare == NULL ) return false;

	// old
	/*
		for (int i = 1; i <= 100; i++) {
		Available[i] = false;
		BAvailable[i] = false;
		FAvailable[i] = false;
	}
	blk32 = SDL_MapRGB( GScreen->format, 0x00, 0x00, 0x00 );
	wht32 = SDL_MapRGB( GScreen->format, 0xFF, 0xFF, 0xFF );

	Sprites[0] = load_image( "Sprites/JinSprites0.png" );
	if ( Sprites[0] == NULL ) return false;
	Sprites[1] = load_image( "Sprites/JinSprites1.png" );
	if ( Sprites[1] == NULL ) return false;

	Sprites[5] = load_image( "Sprites/BatSprites0.png" );
	if ( Sprites[5] == NULL ) return false;

	Sprites[10] = load_image( "Sprites/EagleSprites.png" );
	if ( Sprites[5] == NULL ) return false;

	Sprites[15] = load_image( "Sprites/DragonSprites.png" );
	if ( Sprites[5] == NULL ) return false;


	Sprites[50] = load_image( "Sprites/Proj01Explode.png" );
	if ( Sprites[50] == NULL ) return false;

	Sprites[55] = load_image( "Sprites/BloodSplash.png" );
	if ( Sprites[55] == NULL ) return false;


	StaticSpr[0] = load_image( "Sprites/Proj01.png" );
	if ( StaticSpr[0] == NULL ) return false;
	StaticSpr[1] = load_image( "Sprites/Proj02.png" );
	if ( StaticSpr[1] == NULL ) return false;
	StaticSpr[2] = load_image( "Sprites/Proj03a.png" );
	if ( StaticSpr[2] == NULL ) return false;
	StaticSpr[3] = load_image( "Sprites/Proj03b.png" );
	if ( StaticSpr[3] == NULL ) return false;
	StaticSpr[4] = load_image( "Sprites/Proj03c.png" );
	if ( StaticSpr[4] == NULL ) return false;
	StaticSpr[5] = load_image( "Sprites/Proj04.png" );
	if ( StaticSpr[5] == NULL ) return false;
	StaticSpr[6] = load_image( "Sprites/Proj05.png" );
	if ( StaticSpr[6] == NULL ) return false;

	Life = load_image( "Sprites/LifeIcon.png" );
	if ( Life == NULL ) return false;
	Over = load_image( "Data/Gameover.png" );
	if ( Over == NULL ) return false;
	Start = load_image( "Data/Start.png" );
	if ( Start == NULL ) return false;
	VictoryImg = load_image( "Data/Victory.png" );
	if ( VictoryImg == NULL ) return false;
	EmptyUnit = load_image( "Data/ChargeUnitEmpty.png" );
	if ( EmptyUnit == NULL ) return false;
	FullUnit = load_image( "Data/ChargeUnitFull.png" );
	if ( FullUnit == NULL ) return false;
	UltiPic = load_image( "Data/Ulti.png" );
	if ( UltiPic == NULL ) return false;
	BG = load_image( "Data/BackgroundTotal.png" );
	if ( BG == NULL ) return false;

	BGM1 = Mix_LoadMUS( "BGM/Main01.ogg" ); 
	if( BGM1 == NULL ) return false;

	DeathSFX = Mix_LoadWAV( "SFX/DeathSFX.wav" );
	if( DeathSFX == NULL ) return false;
	UltiVoice[0] = Mix_LoadWAV( "SFX/UltiVoice1.wav" );
	if( UltiVoice[0] == NULL ) return false;
	UltiVoice[1] = Mix_LoadWAV( "SFX/UltiVoice2.wav" );
	if( UltiVoice[1] == NULL ) return false;
	UltiSFX = Mix_LoadWAV( "SFX/UltiSFX.wav" );
	if( UltiSFX == NULL ) return false;
	DamageSFX = Mix_LoadWAV( "SFX/DamageSFX.wav" );
	if( DamageSFX == NULL ) return false;
	*/
	return true;
}

bool SpriteClips() {
	/*int CurType;
	//-------- type 0: Character Sprites -----------
	CurType = 0;

    SPclips[ CLIP_MAXSTATES_spr*CurType + 0 ].x = 0;
    SPclips[ CLIP_MAXSTATES_spr*CurType + 0 ].y = 0;
    SPclips[ CLIP_MAXSTATES_spr*CurType + 0 ].w = 49;
    SPclips[ CLIP_MAXSTATES_spr*CurType + 0 ].h = 34;

    SPclips[ CLIP_MAXSTATES_spr*CurType + 1 ].x = 60;
    SPclips[ CLIP_MAXSTATES_spr*CurType + 1 ].y = 0;
    SPclips[ CLIP_MAXSTATES_spr*CurType + 1 ].w = 49;
    SPclips[ CLIP_MAXSTATES_spr*CurType + 1 ].h = 34;

    SPclips[ CLIP_MAXSTATES_spr*CurType + 2 ].x = 0;
    SPclips[ CLIP_MAXSTATES_spr*CurType + 2 ].y = 40;
    SPclips[ CLIP_MAXSTATES_spr*CurType + 2 ].w = 49;
    SPclips[ CLIP_MAXSTATES_spr*CurType + 2 ].h = 34;

    SPclips[ CLIP_MAXSTATES_spr*CurType + 3 ].x = 60;
    SPclips[ CLIP_MAXSTATES_spr*CurType + 3 ].y = 40;
    SPclips[ CLIP_MAXSTATES_spr*CurType + 3 ].w = 49;
    SPclips[ CLIP_MAXSTATES_spr*CurType + 3 ].h = 34;

	CurType = 1;

    SPclips[ CLIP_MAXSTATES_spr*CurType + 0 ].x = 0;
    SPclips[ CLIP_MAXSTATES_spr*CurType + 0 ].y = 0;
    SPclips[ CLIP_MAXSTATES_spr*CurType + 0 ].w = 60;
    SPclips[ CLIP_MAXSTATES_spr*CurType + 0 ].h = 47;

    SPclips[ CLIP_MAXSTATES_spr*CurType + 1 ].x = 60;
    SPclips[ CLIP_MAXSTATES_spr*CurType + 1 ].y = 0;
    SPclips[ CLIP_MAXSTATES_spr*CurType + 1 ].w = 60;
    SPclips[ CLIP_MAXSTATES_spr*CurType + 1 ].h = 47;

    SPclips[ CLIP_MAXSTATES_spr*CurType + 2 ].x = 120;
    SPclips[ CLIP_MAXSTATES_spr*CurType + 2 ].y = 0;
    SPclips[ CLIP_MAXSTATES_spr*CurType + 2 ].w = 60;
    SPclips[ CLIP_MAXSTATES_spr*CurType + 2 ].h = 47;

    SPclips[ CLIP_MAXSTATES_spr*CurType + 3 ].x = 180;
    SPclips[ CLIP_MAXSTATES_spr*CurType + 3 ].y = 0;
    SPclips[ CLIP_MAXSTATES_spr*CurType + 3 ].w = 60;
    SPclips[ CLIP_MAXSTATES_spr*CurType + 3 ].h = 47;

	SPclips[ CLIP_MAXSTATES_spr*CurType + 4 ].x = 0;
    SPclips[ CLIP_MAXSTATES_spr*CurType + 4 ].y = 47;
    SPclips[ CLIP_MAXSTATES_spr*CurType + 4 ].w = 60;
    SPclips[ CLIP_MAXSTATES_spr*CurType + 4 ].h = 47;

	SPclips[ CLIP_MAXSTATES_spr*CurType + 5 ].x = 60;
    SPclips[ CLIP_MAXSTATES_spr*CurType + 5 ].y = 47;
    SPclips[ CLIP_MAXSTATES_spr*CurType + 5 ].w = 60;
    SPclips[ CLIP_MAXSTATES_spr*CurType + 5 ].h = 47;

	SPclips[ CLIP_MAXSTATES_spr*CurType + 6 ].x = 120;
    SPclips[ CLIP_MAXSTATES_spr*CurType + 6 ].y = 47;
    SPclips[ CLIP_MAXSTATES_spr*CurType + 6 ].w = 60;
    SPclips[ CLIP_MAXSTATES_spr*CurType + 6 ].h = 47;

	CurType = 2;

    SPclips[ CLIP_MAXSTATES_spr*CurType + 0 ].x = 0;
    SPclips[ CLIP_MAXSTATES_spr*CurType + 0 ].y = 0;
    SPclips[ CLIP_MAXSTATES_spr*CurType + 0 ].w = 76;
    SPclips[ CLIP_MAXSTATES_spr*CurType + 0 ].h = 95;

    SPclips[ CLIP_MAXSTATES_spr*CurType + 1 ].x = 76;
    SPclips[ CLIP_MAXSTATES_spr*CurType + 1 ].y = 0;
    SPclips[ CLIP_MAXSTATES_spr*CurType + 1 ].w = 76;
    SPclips[ CLIP_MAXSTATES_spr*CurType + 1 ].h = 94;

    SPclips[ CLIP_MAXSTATES_spr*CurType + 2 ].x = 0;
    SPclips[ CLIP_MAXSTATES_spr*CurType + 2 ].y = 94;
    SPclips[ CLIP_MAXSTATES_spr*CurType + 2 ].w = 76;
    SPclips[ CLIP_MAXSTATES_spr*CurType + 2 ].h = 94;

    SPclips[ CLIP_MAXSTATES_spr*CurType + 3 ].x = 76;
    SPclips[ CLIP_MAXSTATES_spr*CurType + 3 ].y = 94;
    SPclips[ CLIP_MAXSTATES_spr*CurType + 3 ].w = 76;
    SPclips[ CLIP_MAXSTATES_spr*CurType + 3 ].h = 94;

	CurType = 3;

    SPclips[ CLIP_MAXSTATES_spr*CurType + 0 ].x = 0;
    SPclips[ CLIP_MAXSTATES_spr*CurType + 0 ].y = 0;
    SPclips[ CLIP_MAXSTATES_spr*CurType + 0 ].w = 95;
    SPclips[ CLIP_MAXSTATES_spr*CurType + 0 ].h = 103;

    SPclips[ CLIP_MAXSTATES_spr*CurType + 1 ].x = 95;
    SPclips[ CLIP_MAXSTATES_spr*CurType + 1 ].y = 0;
    SPclips[ CLIP_MAXSTATES_spr*CurType + 1 ].w = 95;
    SPclips[ CLIP_MAXSTATES_spr*CurType + 1 ].h = 103;

    SPclips[ CLIP_MAXSTATES_spr*CurType + 2 ].x = 0;
    SPclips[ CLIP_MAXSTATES_spr*CurType + 2 ].y = 103;
    SPclips[ CLIP_MAXSTATES_spr*CurType + 2 ].w = 95;
    SPclips[ CLIP_MAXSTATES_spr*CurType + 2 ].h = 103;

    SPclips[ CLIP_MAXSTATES_spr*CurType + 3 ].x = 95;
    SPclips[ CLIP_MAXSTATES_spr*CurType + 3 ].y = 103;
    SPclips[ CLIP_MAXSTATES_spr*CurType + 3 ].w = 95;
    SPclips[ CLIP_MAXSTATES_spr*CurType + 3 ].h = 103;

	CurType = 10;

    SPclips[ CLIP_MAXSTATES_spr*CurType + 0 ].x = 0;
    SPclips[ CLIP_MAXSTATES_spr*CurType + 0 ].y = 0;
    SPclips[ CLIP_MAXSTATES_spr*CurType + 0 ].w = 30;
    SPclips[ CLIP_MAXSTATES_spr*CurType + 0 ].h = 30;

    SPclips[ CLIP_MAXSTATES_spr*CurType + 1 ].x = 30;
    SPclips[ CLIP_MAXSTATES_spr*CurType + 1 ].y = 0;
    SPclips[ CLIP_MAXSTATES_spr*CurType + 1 ].w = 30;
    SPclips[ CLIP_MAXSTATES_spr*CurType + 1 ].h = 30;

    SPclips[ CLIP_MAXSTATES_spr*CurType + 2 ].x = 0;
    SPclips[ CLIP_MAXSTATES_spr*CurType + 2 ].y = 30;
    SPclips[ CLIP_MAXSTATES_spr*CurType + 2 ].w = 30;
    SPclips[ CLIP_MAXSTATES_spr*CurType + 2 ].h = 30;

    SPclips[ CLIP_MAXSTATES_spr*CurType + 3 ].x = 30;
    SPclips[ CLIP_MAXSTATES_spr*CurType + 3 ].y = 30;
    SPclips[ CLIP_MAXSTATES_spr*CurType + 3 ].w = 30;
    SPclips[ CLIP_MAXSTATES_spr*CurType + 3 ].h = 30;
	//-------- type 1: Projectile Sprites -----------
	ProjClips[0].x = 0;
	ProjClips[0].y = 0;
	ProjClips[0].w = 20;
	ProjClips[0].h = 20;

	ProjClips[1].x = 0;
	ProjClips[1].y = 0;
	ProjClips[1].w = 12;
	ProjClips[1].h = 12;

	ProjClips[2].x = 0;
	ProjClips[2].y = 0;
	ProjClips[2].w = 30;
	ProjClips[2].h = 20;

	ProjClips[3].x = 0;
	ProjClips[3].y = 0;
	ProjClips[3].w = 30;
	ProjClips[3].h = 20;

	ProjClips[4].x = 0;
	ProjClips[4].y = 0;
	ProjClips[4].w = 30;
	ProjClips[4].h = 20;

	ProjClips[5].x = 0;
	ProjClips[5].y = 0;
	ProjClips[5].w = 30;
	ProjClips[5].h = 30;

	ProjClips[6].x = 0;
	ProjClips[6].y = 0;
	ProjClips[6].w = 60;
	ProjClips[6].h = 28;


	LifeRect.x = 0;
	LifeRect.y = 0;
	LifeRect.w = 15;
	LifeRect.h = 15;

	OverRect.x = 0;
	OverRect.y = 0;
	OverRect.w = 300;
	OverRect.h = 200;

	BGrect.x = 0;
	BGrect.y = 0;
	BGrect.w = 640;
	BGrect.h = 480;*/
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

	// start loading game data
	if (!PreloadData()) {
		printf("Preloading data failed!\n");
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
	//mut=SDL_CreateMutex();

	//NewGame();
	
	while(!GE->EndGame) {
		fps.start();
		ApplyBackground();
		/*
		if (hero->cooldown > 0) {
			hero->cooldown--;
			if (hero->cooldown == 0)
				hero->SetAnimation(0);
		}

		if ((hero->alive)&&(!UltiGo)&&(!Victory)) {
			hero->HandleInput();
		}
		hero->Refresh();
		hero->Move();

		for (int i = 1; i <= 20; i++) {
			if (Available[i]) {
				if (Enemies[i]->alive) {
					if (!UltiGo)
						Enemies[i]->Move();
					Enemies[i]->Refresh();
				} else {
					Available[i] = false;
					delete Enemies[i];
				}
			}
		}

		for (int i = 1; i <= 20; i++) {
			if (BAvailable[i]) {
				if (Bullets[i]->alive) {
					if (!UltiGo)
						Bullets[i]->Fly();
				} else {
					BAvailable[i] = false;
					delete Bullets[i];
					Bullets[i] = nullptr;
				}
			}
		}

		for (int i = 1; i <= 10; i++) {
			if (FAvailable[i]) {
				if (Effects[i]->alive) {
					Effects[i]->Refresh();
				} else {
					FAvailable[i] = false;
					delete Effects[i];
					Effects[i] = nullptr;
				}
			}
		}

		for (int i = 1; i <= hero->life; i++) {
			apply_surface(i*20-10, 0, Life, GScreen, &LifeRect);
		}

		DisplayScore();

		if (ItsOver) 
			apply_surface(170, 140, Over, GScreen, &OverRect);
		if (StartingGame) 
			apply_surface(170, 140, Start, GScreen, &OverRect);
		if ((Victory)&&(!ItsOver))
			apply_surface(170, 140, VictoryImg, GScreen, &OverRect);
		if (UltiGo) {
			UltiX+=7;
			apply_surface(UltiX, UltiY, UltiPic, GScreen, &OverRect);
		}
		*/
		//Update the screen
		if( SDL_Flip( GE->Screen ) == -1 ) return 1;
	
		FPScount++; //increment frame counter
		if( (FPScap) && ( fps.get_ticks() < 1000/FRAMES_PER_SECOND ) ) {
			//Sleep the remaining frame time 
			SDL_Delay( (1000/FRAMES_PER_SECOND) - fps.get_ticks() );
		}
	}
	return 0;
}