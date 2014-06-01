#include "main.h"

bool init() {
    if( SDL_Init( SDL_INIT_EVERYTHING ) == -1 )
        return false;
	
    screen = SDL_SetVideoMode( SCREEN_WIDTH+SIDEBAR_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE );
    if( screen == NULL )
        return false;

	SDL_SysWMinfo i;
	SDL_VERSION( &i.version );
	if ( SDL_GetWMInfo ( &i) ) {
		HWND hwnd = i.window;
		int x = (GetSystemMetrics(SM_CXSCREEN)-SCREEN_WIDTH-SIDEBAR_WIDTH)/2;
		int y = (GetSystemMetrics(SM_CYSCREEN)-SCREEN_HEIGHT)/2;
		SetWindowPos( hwnd, HWND_TOP, x, y, SCREEN_WIDTH+SIDEBAR_WIDTH, SCREEN_HEIGHT, NULL );
	}

	if( TTF_Init() == -1 )
        return false;

	if( Mix_OpenAudio( 22050, MIX_DEFAULT_FORMAT, 2, 512 ) == -1 ) 
		return false;

	if (Mix_Init(MIX_INIT_OGG) == 0) 
		return false;

    SDL_WM_SetCaption( "Scrollshooter v.1.00", NULL );

	// load configuration file
	MUSIC_VOLUME = 5;
	SFX_VOLUME = 5;
	LANG_FOLDER = new char[10];

	char cCurrentPath[FILENAME_MAX];
	 if (!_getcwd(cCurrentPath, sizeof(cCurrentPath)))
		return errno;

	cCurrentPath[sizeof(cCurrentPath) - 1] = '\0';
	char retStr[512] = {0};
    char* defStr     = "NULL";

	char* secName = new char[50];
    char* keyName = new char[50];
    char* fileName = cCurrentPath;
	strcat_s(fileName, 512, "\\config.ini");
 
	secName  = "Settings";
	keyName = "lang";
	GetPrivateProfileStringA(secName, keyName, defStr, retStr, 512, fileName);
	strcpy_s(LANG_FOLDER, 10, retStr);

	keyName = "music";
	GetPrivateProfileStringA(secName, keyName, defStr, retStr, 512, fileName);
	MUSIC_VOLUME = atoi(retStr);

	keyName = "sound";
	GetPrivateProfileStringA(secName, keyName, defStr, retStr, 512, fileName);
	SFX_VOLUME = atoi(retStr);

	Mix_VolumeMusic(10*MUSIC_VOLUME);
	Mix_Volume(-1, 10*SFX_VOLUME);

    return true;
}

bool load_files() {
	std::string tmp = std::string(LANG_FOLDER);
	std::string path = "Data/"+tmp;
	// load button sheets
    NEWGAMEbuttonsheet = load_image(path+"/NewGame.png");
    if ( NEWGAMEbuttonsheet == NULL ) return false;

	LEADERbuttonsheet = load_image(path+"/Leaderboard.png");
    if ( LEADERbuttonsheet == NULL ) return false;

	SETTINGSbuttonsheet = load_image(path+"/Settings.png");
    if ( SETTINGSbuttonsheet == NULL ) return false;

	CLOSEbuttonsheet = load_image(path+"/Close.png");
    if ( CLOSEbuttonsheet == NULL ) return false;

	HELPbuttonsheet = load_image(path+"/Help.png");
    if ( HELPbuttonsheet == NULL ) return false;

	EXITbuttonsheet = load_image(path+"/Exit.png");
    if ( EXITbuttonsheet == NULL ) return false;

	ButtonLeft = load_image("Data/ArrowLeft.png");
    if ( ButtonLeft == NULL ) return false;

	ButtonRight = load_image("Data/ArrowRight.png");
    if ( ButtonRight == NULL ) return false;
	
	// load BG's
	MainMenuBG = load_image( "Data/MainMenuBackground.png" );
	if ( MainMenuBG == NULL ) return false;
	LeaderWindow = load_image( "Data/LeaderWindow.png" );
	if ( LeaderWindow == NULL ) return false;
	SettingsBoard = load_image( "Data/LeaderWindow.png" );
	if ( SettingsBoard == NULL ) return false;
	SettingsWindow = load_image( "Data/LeaderWindow.png" );
	if ( SettingsWindow == NULL ) return false;
	HelpWindow = load_image( "Data/LeaderWindow.png" );
	if ( HelpWindow == NULL ) return false;


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
void WindowClose() {
	if (STActivated) {
		// if settings were activated, closing the window will cause
		// to save changed values in .ini-file
		 char cCurrentPath[FILENAME_MAX];
		 if (!_getcwd(cCurrentPath, sizeof(cCurrentPath)))
			return;
		cCurrentPath[sizeof(cCurrentPath) - 1] = '\0';
		char* fileName = cCurrentPath;
		strcat_s(fileName, 512, "\\config.ini");
		char* secName = new char[50];
		char* keyName = new char[50];
		char* dataVal = new char[50];

		secName  = "Settings";
		keyName = "lang";
		WritePrivateProfileStringA(secName, keyName, LANG_FOLDER, fileName);
		keyName = "music";
		sprintf_s(dataVal, sizeof(dataVal), "%d", MUSIC_VOLUME);
		WritePrivateProfileStringA(secName, keyName, dataVal, fileName);
		keyName = "sound";
		sprintf_s(dataVal, sizeof(dataVal), "%d", SFX_VOLUME);
		WritePrivateProfileStringA(secName, keyName, dataVal, fileName);
		// and change sfx/music volume while at it
		Mix_VolumeMusic(10*MUSIC_VOLUME);
		Mix_Volume(-1, 10*SFX_VOLUME);
	}
	LBActivated = false;
	STActivated = false;
	HLPactivated = false;
	Mix_PlayChannel( -1, BasicClick, 0 );
}

void LeaderWindowDisplay() {
	LBActivated = true;
	STActivated = false;
	HLPactivated = false;
	Mix_PlayChannel( -1, BasicClick, 0 );
}
//=====================================================================================================================
void SettingsWindowDisplay() {
	STActivated = true;
	LBActivated = false;
	HLPactivated = false;
	Mix_PlayChannel( -1, BasicClick, 0 );
}
//=====================================================================================================================
void HelpWindowDisplay() {
	LBActivated = false;
	STActivated = false;
	HLPactivated = true;
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
	Mix_VolumeMusic(10*MUSIC_VOLUME);
	Mix_Volume(-1, 10*SFX_VOLUME);
	GameStarted = true;
	SDL_Delay(100);
	SDL_FillRect(screen, &screen->clip_rect, SDL_MapRGB( screen->format, 0x00, 0x00, 0x00 ) );
	if( SDL_Flip( screen ) == -1 ) return 0;
	
	printf("Before entity!\n");
	game = new GameEntity(GameThread, screen);
	// pass language folder to game entity
	std::string tmp = std::string(LANG_FOLDER);
	std::string path = "Data/"+tmp;
	game->LanguageFolder = path;
	// ...
	printf("After entity!\n");
	game->StartGame();
	printf("After gamestart!\n");
	SDL_WaitThread(game->Thread, NULL);
	//GameQuit = true;
	return 0;
}
//=====================================================================================================================
int CreateLeaderboard() {
	int count = -1;
	 char cCurrentPath[FILENAME_MAX];
	 if (!_getcwd(cCurrentPath, sizeof(cCurrentPath)))
		return errno;

	cCurrentPath[sizeof(cCurrentPath) - 1] = '\0';

	strcat(cCurrentPath, "\\records.ini");
	Leaderboard *lb = new Leaderboard(cCurrentPath, LB_SIZE);
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

int CreateHelp() {
	int count = -1;
	std::string path = "Data\\"+std::string(LANG_FOLDER);
	path = path+"\\Help.txt";
	std::ifstream text(path);
	std::string line;

	while(std::getline(text, line)) {
        HelpMessagePart[++count] = TTF_RenderText_Solid( FNT_BrushM, line.c_str(), textColor );
		if (count >= 8)
			break;
    }

	for (int i = 0; i <= count; i++) 
			apply_surface( 20, 15+i*35, HelpMessagePart[i], HelpWindow );

	return true;
}

int CreateSettings() {
	int count = -1;
	std::string path = "Data\\"+std::string(LANG_FOLDER);
	path = path+"\\Settings.txt";
	std::ifstream text(path);
	std::string line;
	// read text from the file
	while(std::getline(text, line)) {
        SettingsMessagePart[++count] = TTF_RenderText_Solid( FNT_BrushM, line.c_str(), textColor );
		if (count >= 8)
			break;
    }
	return true;
}

int RedrawSettings() {
	std::stringstream ss;
	// read and draw sfx volume
	ss << SFX_VOLUME;
	std::string line = ss.str();
	SDL_FreeSurface(SettingsMessagePart[10]);
	SettingsMessagePart[10] = TTF_RenderText_Solid(FNT_BrushM, line.c_str(), textColor );
	ss.str("");
	ss.clear();
	// read and draw music volume
	ss << MUSIC_VOLUME;
	line = ss.str();
	SDL_FreeSurface(SettingsMessagePart[11]);
	SettingsMessagePart[11] = TTF_RenderText_Solid(FNT_BrushM, line.c_str(), textColor );
	ss.clear();
	// read and draw music volume
	line = std::string(LANG_FOLDER);
	SDL_FreeSurface(SettingsMessagePart[12]);
	SettingsMessagePart[12] = TTF_RenderText_Solid(FNT_BrushM, line.c_str(), textColor );
	ss.clear();
	// draw texts
	for (int i = 0; i <= 8; i++) 
		apply_surface( 20, 15+i*35, SettingsMessagePart[i], SettingsWindow );
	// sound
	apply_surface( 225, 17, SettingsMessagePart[10], SettingsWindow );
	// music
	apply_surface( 225, 52, SettingsMessagePart[11], SettingsWindow );
	// language
	apply_surface( 220, 85, SettingsMessagePart[12], SettingsWindow );
	return true;
}
//=====================================================================================================================
void SoundVolumeUp() {
	if (SFX_VOLUME < 9)
		SFX_VOLUME++;
}

void SoundVolumeDown() {
	if (SFX_VOLUME > 0)
		SFX_VOLUME--;
}

void MusicVolumeUp() {
	if (MUSIC_VOLUME < 9)
		MUSIC_VOLUME++;
}

void MusicVolumeDown() {
	if (MUSIC_VOLUME > 0)
		MUSIC_VOLUME--;
}

void LanguageChangeLeft() {
	if (LANG_FOLDER == "RU")
		LANG_FOLDER = "EN";
	else
		LANG_FOLDER = "RU";
}

void LanguageChangeRight() {
	if (LANG_FOLDER == "EN")
		LANG_FOLDER = "RU";
	else
		LANG_FOLDER = "EN";
}
//=====================================================================================================================
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

	if( CreateHelp() == false )
		return 4;

	if( CreateSettings() == false )
		return 5;

    set_clips();
    Button NGbtn( 280, 40, 180, 90, NEWGAMEbuttonsheet, &SubAct, 0);
	Button LDRbtn( 280, 140, 180, 90,  LEADERbuttonsheet, &LeaderWindowDisplay, 0);
	Button HLPbtn( 280, 240, 180, 90,  HELPbuttonsheet, &HelpWindowDisplay, 0);
	Button OPTbtn( 280, 340, 180, 90,  SETTINGSbuttonsheet, &SettingsWindowDisplay, 0);
	Button EXITbtn( 280, 440, 180, 90,  EXITbuttonsheet, &QuitGame, 0);
	Button CLSbtn( 330, 402, 120, 50,  CLOSEbuttonsheet, &WindowClose, 1);

	Button SFXLeft( 425, 82, 30, 30, ButtonLeft, &SoundVolumeDown, 2);
	Button SFXRight( 485, 82, 30, 30, ButtonRight, &SoundVolumeUp, 2);
	Button MUSLeft( 425, 117, 30, 30, ButtonLeft, &MusicVolumeDown, 2);
	Button MUSRight( 485, 117, 30, 30, ButtonRight, &MusicVolumeUp, 2);
	Button LANGLeft( 425, 150, 30, 30, ButtonLeft, &LanguageChangeLeft, 2);
	Button LANGRight( 485, 150, 30, 30, ButtonRight, &LanguageChangeRight, 2);

	LBActivated = false;
	GameStarted = false;
	GameQuit = false;

	Mix_PlayMusic( TitleBGM, -1 );

	while(GameQuit == false) {
        if( SDL_PollEvent( &ExitLoop ) ) {
			if (!GameStarted) {
				// handle button events
				if (!LBActivated && !STActivated && !HLPactivated) {
					NGbtn.handle_events(&ExitLoop);
					LDRbtn.handle_events(&ExitLoop);
					OPTbtn.handle_events(&ExitLoop);
					HLPbtn.handle_events(&ExitLoop);
					EXITbtn.handle_events(&ExitLoop);
				} else {
					CLSbtn.handle_events(&ExitLoop);
				}

				if (STActivated) {
					apply_surface(0, 0, SettingsBoard, SettingsWindow );
					SFXLeft.handle_events(&ExitLoop);
					SFXRight.handle_events(&ExitLoop);
					MUSLeft.handle_events(&ExitLoop);
					MUSRight.handle_events(&ExitLoop);
					LANGLeft.handle_events(&ExitLoop);
					LANGRight.handle_events(&ExitLoop);
				}

				// Fill the screen white
				apply_surface(0, 0, MainMenuBG, screen);
				NGbtn.show(screen);
				OPTbtn.show(screen);
				LDRbtn.show(screen);
				EXITbtn.show(screen);
				HLPbtn.show(screen);
				if (LBActivated) {
					apply_surface( 240, 70, LeaderWindow, screen );
					CLSbtn.show(screen);
				}
				if (STActivated) {
					RedrawSettings();
					apply_surface( 240, 70, SettingsWindow, screen );
					CLSbtn.show(screen);
					SFXLeft.show(screen);
					SFXRight.show(screen);
					MUSLeft.show(screen);
					MUSRight.show(screen);
					LANGLeft.show(screen);
					LANGRight.show(screen);
				}
				if (HLPactivated) {
					apply_surface( 240, 70, HelpWindow, screen );
					CLSbtn.show(screen);
				}
			// Update the screen
			if( SDL_Flip( screen ) == -1 )
            return 1;
			}
			// Handle single button press
			if( ExitLoop.type == SDL_KEYDOWN ) {
				//if ((ExitLoop.key.keysym.sym == BUTTON_SHOOT)||(ExitLoop.key.keysym.sym == BUTTON_ULTI)||(ExitLoop.key.keysym.sym == BUTTON_QUIT) ) {
					game->HandleButtonPress(ExitLoop.key.keysym.sym);
					if (ExitLoop.key.keysym.sym == BUTTON_QUIT) {
						GameQuit = true;
						break;
					}
			//	}
			}

			if (ExitLoop.type == SDL_QUIT ) {
				GameQuit = true;
				break;
			}
		}
	}
	// if the game was finished, save the leaderboard
	if (game->Victory || game->Defeat) {
		char cCurrentPath[FILENAME_MAX];
		if (!_getcwd(cCurrentPath, sizeof(cCurrentPath)))
			return errno;
		cCurrentPath[sizeof(cCurrentPath) - 1] = '\0';
		strcat(cCurrentPath, "\\records.ini");
		Leaderboard *lb = new Leaderboard(cCurrentPath, LB_SIZE);
		lb->Update();

		std::string name = game->PlayerName;
		int GameScore = game->Score;
		int GameLives = game->Hero->Health;
		int pos = 0;
		for (int i = lb->recordcount; i >= 0; i--) {
			if (lb->StatValue[i-1][0] <= GameScore)
				pos = i;
		}
		if (GameScore >= lb->StatValue[lb->recordcount-1][0] && name != "") {
			for (int i = lb->recordcount-1; i > pos; i--) {
				lb->StatValue[i][0] = lb->StatValue[i-1][0];
				lb->StatValue[i][1] = lb->StatValue[i-1][1];
				lb->PlayerName[i] = lb->PlayerName[i-1];
			}
			lb->StatValue[pos][0] = GameScore;
			lb->StatValue[pos][1] = GameLives;
			char *y = new char[name.length() + 1];
			std::strcpy(y, name.c_str());
			lb->PlayerName[pos] = y;
			lb->Save();
		}
	}
	//

	game->EndGame = true;
	clean_up();
    return 0;
}