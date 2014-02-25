#ifndef __IMAGEWORK_H__
#define __IMAGEWORK_H__

//includes

#include "SDL.h"
#include <stdlib.h>
#include <string>

// ----------------   button class declaration   ---------------------------
class Button
{
    private:
    SDL_Rect box;
    SDL_Rect* clip;
	SDL_Surface* sheet;
	int Btype;
	void (*Func)();

    public:
    Button( int x, int y, int w, int h, SDL_Surface* bsht, void (*onClick)(), int layout);
    void handle_events(SDL_Event *evnt);
    void show(SDL_Surface *scr);
};

SDL_Surface *load_image( std::string filename );
SDL_Surface *load_image_nc( std::string filename );
void apply_surface( int x, int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip = NULL );
void set_clips();
bool check_collision( SDL_Rect A, SDL_Rect B );

#endif
