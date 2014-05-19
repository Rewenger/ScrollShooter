#include "SDL.h"
#include "SDL_image.h"
#ifndef Imagework
#include "Imagework.h"
#endif
#include <string>

//constants
const int CLIP_MOUSEOVER = 0;
const int CLIP_MOUSEOUT = 1;
const int CLIP_MOUSEDOWN = 2;
const int CLIP_MOUSEUP = 3;
const int CLIP_MAXSTATES = 10;

//variables
SDL_Rect clips[100];

// ----------------   functions for images   ---------------------------
SDL_Surface *load_image( std::string filename )
{
    SDL_Surface* loadedImage = NULL;
    SDL_Surface* optimizedImage = NULL;
    loadedImage = IMG_Load( filename.c_str() );
    if( loadedImage != NULL ) {
        optimizedImage = SDL_DisplayFormatAlpha( loadedImage );
        SDL_FreeSurface( loadedImage );
    }
    return optimizedImage;
}

SDL_Surface *load_image_nc( std::string filename )
{
    SDL_Surface* loadedImage = NULL;
    SDL_Surface* optimizedImage = NULL;
    loadedImage = IMG_Load( filename.c_str() );
    if( loadedImage != NULL )
    {
        optimizedImage = SDL_DisplayFormat( loadedImage );

        SDL_FreeSurface( loadedImage );
    }
    return optimizedImage;
}

void apply_surface( int x, int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip)
{
    SDL_Rect offset;
    offset.x = x;
    offset.y = y;
    SDL_BlitSurface( source, clip, destination, &offset );
}

void set_clips()
{
	int CurType;
	//-------- type 0: Large button, 180*90 -----------
	CurType = 0;

    clips[ CLIP_MOUSEOVER + CLIP_MAXSTATES*CurType].x = 0;
    clips[ CLIP_MOUSEOVER + CLIP_MAXSTATES*CurType].y = 0;
    clips[ CLIP_MOUSEOVER + CLIP_MAXSTATES*CurType].w = 180;
    clips[ CLIP_MOUSEOVER + CLIP_MAXSTATES*CurType].h = 90;

    clips[ CLIP_MOUSEOUT + CLIP_MAXSTATES*CurType].x = 0;
    clips[ CLIP_MOUSEOUT + CLIP_MAXSTATES*CurType].y = 90;
    clips[ CLIP_MOUSEOUT + CLIP_MAXSTATES*CurType].w = 180;
    clips[ CLIP_MOUSEOUT + CLIP_MAXSTATES*CurType].h = 90;
	//-------- type 1: Medium button, 120*50 -----------
	CurType = 1;
	clips[ CLIP_MOUSEOVER + CLIP_MAXSTATES*CurType].x = 0;
    clips[ CLIP_MOUSEOVER + CLIP_MAXSTATES*CurType].y = 0;
    clips[ CLIP_MOUSEOVER + CLIP_MAXSTATES*CurType].w = 120;
    clips[ CLIP_MOUSEOVER + CLIP_MAXSTATES*CurType].h = 50;

    clips[ CLIP_MOUSEOUT + CLIP_MAXSTATES*CurType].x = 0;
    clips[ CLIP_MOUSEOUT + CLIP_MAXSTATES*CurType].y = 50;
    clips[ CLIP_MOUSEOUT + CLIP_MAXSTATES*CurType].w = 120;
    clips[ CLIP_MOUSEOUT + CLIP_MAXSTATES*CurType].h = 50;
}

Button::Button( int x, int y, int w, int h, SDL_Surface* bsht, void (*onClk)(), int layout = 0)
{
    box.x = x;
    box.y = y;
    box.w = w;
    box.h = h;
    clip = &clips[ CLIP_MOUSEOUT + CLIP_MAXSTATES*layout];
	sheet = bsht;
	Func = onClk;
	Btype = layout;
}

void Button::handle_events(SDL_Event *evnt)
{
    int x = 0, y = 0;
	// mouse over button
    if( (*evnt).type == SDL_MOUSEMOTION ) {
        x = (*evnt).motion.x;
        y = (*evnt).motion.y;

        if( ( x > box.x ) && ( x < box.x + box.w ) && ( y > box.y ) && ( y < box.y + box.h ) ) {
            clip = &clips[CLIP_MOUSEOVER + CLIP_MAXSTATES*Btype];
        } else {
			clip = &clips[CLIP_MOUSEOUT + CLIP_MAXSTATES*Btype];
        }
    }
	// mouse button pressed
    if( (*evnt).type == SDL_MOUSEBUTTONDOWN ) {
        if( (*evnt).button.button == SDL_BUTTON_LEFT ) {
            x = (*evnt).button.x;
            y = (*evnt).button.y;

            if( ( x > box.x ) && ( x < box.x + box.w ) && ( y > box.y ) && ( y < box.y + box.h ) )
				Func();
        }
    }
    //If a mouse button was released
    if( (*evnt).type == SDL_MOUSEBUTTONUP ) {
        if( (*evnt).button.button == SDL_BUTTON_LEFT ) {
            x = (*evnt).button.x;
            y = (*evnt).button.y;

            if( ( x > box.x ) && ( x < box.x + box.w ) && ( y > box.y ) && ( y < box.y + box.h ) ) {
                //Set the button sprite
                //clip = &clips[ CLIP_MOUSEUP ];
            }
        }
    }
}

void Button::show(SDL_Surface *scr)
{
    apply_surface( box.x, box.y, sheet, scr, clip );
}

bool check_collision( SDL_Rect A, SDL_Rect B )
{
    //The sides of the rectangles
    int leftA, leftB;
    int rightA, rightB;
    int topA, topB;
    int bottomA, bottomB;

    //Calculate the sides of rect A
    leftA = A.x;
    rightA = A.x + A.w;
    topA = A.y;
    bottomA = A.y + A.h;

    //Calculate the sides of rect B
    leftB = B.x;
    rightB = B.x + B.w;
    topB = B.y;
    bottomB = B.y + B.h;

    //If any of the sides from A are outside of B
    if( bottomA <= topB )
    {
        return false;
    }

    if( topA >= bottomB )
    {
        return false;
    }

    if( rightA <= leftB )
    {
        return false;
    }

    if( leftA >= rightB )
    {
        return false;
    }

    //If none of the sides from A are outside B
    return true;
}