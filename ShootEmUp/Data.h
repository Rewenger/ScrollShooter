#ifndef __DATA_H__
#define __DATA_H__

#pragma once

#include "SDL.h"

//Screen attributes

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int SCREEN_BPP = 32;

// other constants
const int PROJ_WIDTH = 20;
const int PROJ_HEIGHT = 20;

// button controls
const int BUTTON_UP = SDLK_w;
const int BUTTON_DOWN = SDLK_s;
const int BUTTON_LEFT = SDLK_a;
const int BUTTON_RIGHT = SDLK_d;
const int BUTTON_SHOOT = SDLK_SPACE;
const int BUTTON_ULTI = SDLK_RETURN;
const int BUTTON_QUIT = SDLK_ESCAPE;
const int FRAMES_PER_SECOND = 30;
const double CONST_PI = 0.017453;


// sprite values
const int Clip_Total = 0;
extern int Clip_ValuesA[100][2],
	Clip_ValuesB[100][2];

void InitClipValues();

#endif