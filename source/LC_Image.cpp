#include "LC_Image.h"

#include <iostream>
using namespace std;

#include "LC_RenderObject.h"
#include <SDL.h>

LC_Image::LC_Image() {
    bg_colour.r = 0;
    bg_colour.g = 0;
    bg_colour.b = 0;
    bg_colour.a = 255;
    key_colour.r = 0;
    key_colour.g = 0;
    key_colour.b = 0;
    key_colour.a = 255;
    image_region.x = 0;
    image_region.y = 0;
    image_region.w = 0;
    image_region.h = 0;
}

LC_Image::~LC_Image() {
    if(texture) {
        SDL_DestroyTexture(texture);
        texture = NULL;
    }
    if(surface) {
        SDL_FreeSurface(surface);
        surface = NULL;
    }
}

void LC_Image::render() {
    if(renderer) {
        if(bg_fill) {
            //cout << image_region.x << " " <<  image_region.y << " " <<  image_region.w << " " <<  image_region.h << endl;
            SDL_SetRenderDrawColor(renderer,bg_colour.r,bg_colour.g,bg_colour.b,bg_colour.a);
            SDL_RenderFillRect(renderer,&image_region);
        }
        if(texture) {
            SDL_RenderCopy(renderer,texture,NULL,&image_region);
        }
        if(border_draw) {
            SDL_SetRenderDrawColor(renderer,border_colour.r,border_colour.g,border_colour.b,border_colour.a);
            SDL_RenderDrawRect(renderer,&image_region);
        }
    }
}

void LC_Image::loadImage(const char* in_filename) {
    if(surface) {
        SDL_FreeSurface(surface);
        surface = NULL;
    }
    if(texture) {
        SDL_DestroyTexture(texture);
        texture = NULL;
    }
    surface = SDL_LoadBMP(in_filename);
    if(surface) {
        //if((r>=0)&&(g>=0)&&(b>=0))
        //    SDL_SetColorKey( this_surface, SDL_TRUE, SDL_MapRGB( this_surface->format, (Uint8)r, (Uint8)g, (Uint8)b ) );
        if(renderer) {
            texture = SDL_CreateTextureFromSurface(renderer,surface);
            if(!texture) {
                cout << SDL_GetError() << endl;
            } else {
                SDL_SetTextureBlendMode(texture,SDL_BLENDMODE_BLEND);
                SDL_SetTextureAlphaMod(texture,(Uint8)texture_alpha);
            }
        }
    } else {
        cout << SDL_GetError() << endl;
    }
}

void LC_Image::refresh() {
    if(surface) {
        if(colour_keying) {
            SDL_SetColorKey( surface, SDL_TRUE, SDL_MapRGB( surface->format, (Uint8)key_colour.r, (Uint8)key_colour.g, (Uint8)key_colour.b ) );
        } else {
            SDL_SetColorKey( surface, SDL_FALSE, SDL_MapRGB( surface->format, (Uint8)key_colour.r, (Uint8)key_colour.g, (Uint8)key_colour.b ) );
        }
        if(renderer) {
            if(texture) {
                SDL_DestroyTexture(texture);
                texture = NULL;
            }
            texture = SDL_CreateTextureFromSurface(renderer,surface);
            if(texture) {
                SDL_SetTextureBlendMode(texture,SDL_BLENDMODE_BLEND);
                SDL_SetTextureAlphaMod(texture,(Uint8)texture_alpha);
            }
        }
    }
}

void LC_Image::setKeyColour(int r, int g, int b) {
    if(surface) {
        if(!colour_keying) {
            SDL_SetColorKey( surface, SDL_FALSE, SDL_MapRGB( surface->format, (Uint8)key_colour.r, (Uint8)key_colour.g, (Uint8)key_colour.b ) );
        }
    }
    key_colour.r = r;
    key_colour.g = g;
    key_colour.b = b;
    key_colour.a = 255;
    refresh();
}

void LC_Image::setRenderer(SDL_Renderer* ir) {
    LC_RenderObject::setRenderer(ir);
    refresh();
}
