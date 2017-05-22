#include "LC_Utils.h"

#include <iostream>
using namespace std;

#include <SDL.h>

SDL_Texture* LC_Utils::LC_LoadTexture(SDL_Renderer* in_renderer, const char* in_filename, int r, int g, int b) {
    SDL_Texture* out = NULL;

    SDL_Surface* this_surface = SDL_LoadBMP(in_filename);
    if(this_surface) {
        if((r>=0)&&(g>=0)&&(b>=0))
            SDL_SetColorKey( this_surface, SDL_TRUE, SDL_MapRGB( this_surface->format, (Uint8)r, (Uint8)g, (Uint8)b ) );
        out = SDL_CreateTextureFromSurface(in_renderer,this_surface);
        if(!out) {
            cout << SDL_GetError() << endl;
        }
        SDL_FreeSurface(this_surface);
    } else {
        cout << SDL_GetError() << endl;
    }


    return out;
}
