#include "main.h"

bool init() {
    if( SDL_Init( SDL_INIT_EVERYTHING ) == -1 )
        return false;
	
    screen = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE );
    if( screen == NULL )
        return false;

	if( TTF_Init() == -1 )
        return false;

	if( Mix_OpenAudio( 22050, MIX_DEFAULT_FORMAT, 2, 512 ) == -1 ) 
		return false;

	if (Mix_Init(MIX_INIT_OGG) == 0) 
		return false;

    SDL_WM_SetCaption( "Scrollshooter 0.01", NULL );
    return true;
}

bool load_files() {
	// load button sheets
    NEWGAMEbuttonsheet = load_image( "Data/NewGameButtons.png" );
    if ( NEWGAMEbuttonsheet == NULL ) return false;

	LEADERbuttonsheet = load_image( "Data/LeaderButtons.png" );
    if ( LEADERbuttonsheet == NULL ) return false;

	SETTINGSbuttonsheet = load_image( "Data/SettingsButtons.png" );
    if ( SETTINGSbuttonsheet == NULL ) return false;

	CLOSEbuttonsheet = load_image( "Data/CloseButtons.png" );
    if ( CLOSEbuttonsheet == NULL ) return false;

	EXITbuttonsheet = load_image( "Data/ExitButtons.png" );
    if ( EXITbuttonsheet == NULL ) return false;
	
	// load BG's
	MainMenuBG = load_image( "Data/MainMenuBackground.png" );
	if ( MainMenuBG == NULL ) return false;
	LeaderWindow = load_image( "Data/LeaderWindow.png" );
	if ( LeaderWindow == NULL ) return false;
	SettingsWindow = load_image( "Data/LeaderWindow.png" );
	if ( SettingsWindow == NULL ) return false;


	// load fonts
	FNT_BrushM = TTF_OpenFont( "Data/Brush_m.ttf", 26 );
	if ( FNT_BrushM == NULL ) return false;

	// load BGM
	TitleBGM = Mix_LoadMUS( "BGM/Neutral.ogg" ); 
	if( TitleBGM == NULL ) return false;

	// load SFX	
	BasicClick = Mix_LoadWAV( "SFX/MenuClick.wav" );
	if( BasicClick == NULL ) return false;

    return true;
}

//=====================================================================================================================
void clean_up() {
	// free surfaces
	SDL_FreeSurface( LEADERbuttonsheet );
    SDL_FreeSurface( NEWGAMEbuttonsheet );
	SDL_FreeSurface( SETTINGSbuttonsheet );
	SDL_FreeSurface( CLOSEbuttonsheet );
	SDL_FreeSurface( EXITbuttonsheet );
	SDL_FreeSurface( MainMenuBG );
	SDL_FreeSurface( screen );

	// free sfx
	Mix_FreeChunk( BasicClick );

	// free bgm
	Mix_FreeMusic( TitleBGM );

	// quit systems
	Mix_CloseAudio();
	Mix_Quit();
    SDL_Quit();
}

//=====================================================================================================================
void HelpWindowDisplay() {
	LBActivated = true;
	Mix_PlayChannel( -1, BasicClick, 0 );
}

void HelpWindowClose() {
	LBActivated = false;
	STActivated = false;	
	Mix_PlayChannel( -1, BasicClick, 0 );
}
//=====================================================================================================================
void SettingsWindowDisplay() {
	STActivated = true;
	Mix_PlayChannel( -1, BasicClick, 0 );
}
//=====================================================================================================================

void QuitGame() {
	Mix_PlayChannel( -1, BasicClick, 0 );
	SDL_Delay(150);
	clean_up();
    exit(0);
}

int SubStartGame(void *data) {
	GameStarted = true;
	SDL_Delay(100);
	SDL_FillRect(screen, &screen->clip_rect, SDL_MapRGB( screen->format, 0x00, 0x00, 0x00 ) );
	if( SDL_Flip( screen ) == -1 ) return 0;
	
	printf("Before entity!\n");
	game = new GameEntity(GameThread, screen);
	printf("After entity!\n");
	game->StartGame();
	printf("After gamestart!\n");
	SDL_WaitThread(game->Thread, NULL);
	//GameQuit = true;
	return 0;
}
//=====================================================================================================================
bool CreateLeaderboard() {
	int count = -1;
	Leaderboard *lb;

	 char cCurrentPath[FILENAME_MAX];
	 if (!_getcwd(cCurrentPath, sizeof(cCurrentPath)))
		return errno;

	cCurrentPath[sizeof(cCurrentPath) - 1] = '\0';

	strcat(cCurrentPath, "\\records.ini");
	lb = new Leaderboard(cCurrentPath, LB_SIZE);
	printf(cCurrentPath);
	lb->Update();

/*	printf("Processing result!\n");
	printf("return = %s \n", lb->PlayerName[0]);
	printf("return = %d \n", lb->StatValue[0][0]);
	printf("return = %d \n", lb->StatValue[0][1]);*/

	char* ProcStr = new char[512];
	char *tempstr = new char[512];

	LeaderMessagePart[++count] = TTF_RenderText_Solid( FNT_BrushM, "Best players:", textColor );
	for (int i = 0; i < LB_SIZE; i++) {
		sprintf(ProcStr, "%d", i+1);
		strcat(ProcStr, ") ");
		strcat(ProcStr, lb->PlayerName[i]);
		strcat(ProcStr, ": ");
		sprintf(tempstr, "%d", lb->StatValue[i][0]);
		strcat(ProcStr, tempstr);
		strcat(ProcStr, " pts, lives: ");
		sprintf(tempstr, "%d", lb->StatValue[i][1]);
		strcat(ProcStr, tempstr);
		LeaderMessagePart[++count] = TTF_RenderText_Solid( FNT_BrushM, ProcStr, textColor );
	}

	for (int i = 0; i <= count; i++) {
		if (LeaderMessagePart[i] == NULL) return 0;
	}

	for (int i = 0; i <= count; i++) 
				apply_surface( 20, 15+i*35, LeaderMessagePart[i], LeaderWindow );

	return 1;
}

void SubAct() {
	printf("SubAct!\n");
	SubThread = SDL_CreateThread( SubStartGame, NULL );
}
//=====================================================================================================================

int main( int argc, char* args[] ) { 
    if( init() == false ) 
        return 1;

    if( load_files() == false )
        return 2;

	if( CreateLeaderboard() == false )
		return 3;

    set_clips();
    Button NGbtn( 230, 50, 180, 90, NEWGAMEbuttonsheet, &SubAct, 0);
	Button HLPbtn( 230, 160, 180, 90,  LEADERbuttonsheet, &HelpWindowDisplay, 0);
	Button OPTbtn( 230, 270, 180, 90,  SETTINGSbuttonsheet, &SettingsWindowDisplay, 0);
	Button EXITbtn( 230, 380, 180, 90,  EXITbuttonsheet, &QuitGame, 0);
	Button CLSbtn( 270, 382, 120, 50,  CLOSEbuttonsheet, &HelpWindowClose, 1);
	LBActivated = false;
	GameStarted = false;
	GameQuit = false;

	Mix_PlayMusic( TitleBGM, -1 );

	while(GameQuit == false) {
        if( SDL_PollEvent( &ExitLoop ) ) {
			if (!GameStarted) {
				if (!LBActivated && !STActivated) {
					NGbtn.handle_events(&ExitLoop);
					HLPbtn.handle_events(&ExitLoop);
					OPTbtn.handle_events(&ExitLoop);
					EXITbtn.handle_events(&ExitLoop);
				} else {
					CLSbtn.handle_events(&ExitLoop);
				}

				// Fill the screen white
				apply_surface(0, 0, MainMenuBG, screen);
				NGbtn.show(screen);
				OPTbtn.show(screen);
				HLPbtn.show(screen);
				EXITbtn.show(screen);
				if (LBActivated) {
					apply_surface( 170, 50, LeaderWindow, screen );
					CLSbtn.show(screen);
				}
				if (STActivated) {
					apply_surface( 170, 50, SettingsWindow, screen );
					CLSbtn.show(screen);
				}
			//Update the screen
			if( SDL_Flip( screen ) == -1 )
            return 1;
			}
			if ( ExitLoop.type == SDL_QUIT ) {
				GameQuit = true;
				break;
			}
		}
	}
	game->EndGame = true;
	clean_up();
    return 0;
}