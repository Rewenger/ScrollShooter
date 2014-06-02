#ifndef __DATA_H__
#define __DATA_H__

#pragma once

#include "SDL.h"
#include <stdlib.h>

//Screen attributes

const int SCREEN_WIDTH = 640;
const int SIDEBAR_WIDTH = 145;
const int SCREEN_HEIGHT = 585;
const int SCREEN_BPP = 32;

// other constants
const int PROJ_WIDTH = 20;
const int PROJ_HEIGHT = 20;
const int HERO_HP_MAX = 20;
const int HERO_CHARGE_MAX = 20;

// button controls
const int BUTTON_UP = SDLK_w;
const int BUTTON_DOWN = SDLK_s;
const int BUTTON_LEFT = SDLK_a;
const int BUTTON_RIGHT = SDLK_d;
const int BUTTON_SHOOT = SDLK_SPACE;
const int BUTTON_ULTI1 = SDLK_q;
const int BUTTON_ULTI2 = SDLK_e;
const int BUTTON_QUIT = SDLK_ESCAPE;
const int FRAMES_PER_SECOND = 30;
const double CONST_PI = 0.017453;

// sprite values
const int  Clip_Total = 3;
const int  Clip_Total_2 = 1;
extern int Clip_ValuesA[100][2],
	       Clip_ValuesB[100][2];


class SpriteData {
public:
	int SpriteSheet;			 // номер листа со спрайтами
	int AnimationMax;			 // конец кадров с анимациями
	SDL_Rect *Clips[25];         // SDL_rect, который можно использовать
	void Init(int w, int h, int rows, int columns, int total); // нарезает входной спрайтлист на клипы
};


void InitClipValues();

#endif