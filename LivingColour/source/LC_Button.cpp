#include "LC_Button.h"

#include <iostream>
using namespace std;

#include <SDL.h>

#include "LC_Utils.h"

LC_Button::LC_Button() {
    bg_colour = new SDL_Colour*[LC_BUTTON_ACTION_TYPE_N];
    bg_image = new SDL_Texture*[LC_BUTTON_ACTION_TYPE_N];
    for(int i = 0; i < LC_BUTTON_ACTION_TYPE_N; i++) {
        bg_colour[i] = NULL;
        bg_image[i] = NULL;
    }
}

LC_Button::~LC_Button() {
    for(int i = 0; i < LC_BUTTON_ACTION_TYPE_N; i++) {
        if(bg_colour[i]) delete bg_colour[i];
        if(bg_image[i]) delete bg_image;
    }
    delete [] bg_colour;
    delete [] bg_image;
}

void LC_Button::setGeometry(double ix, double iy, double iw, double ih){
    x = ix;
    y = iy;
    w = iw;
    h = ih;
}

void LC_Button::setColour(int r, int g, int b, int a, LC_ButtonActionType t) {
    if(!bg_colour[t]) {
        bg_colour[t] = new SDL_Colour;
    }
    bg_colour[t]->r = r;
    bg_colour[t]->g = g;
    bg_colour[t]->b = b;
    bg_colour[t]->a = a;
}

void LC_Button::setColour(int r, int g, int b, int a) {
    for(int i = 0; i < LC_BUTTON_ACTION_TYPE_N; i++) {
        setColour(r,g,b,a);
    }
}

void LC_Button::setImage(SDL_Renderer* ir, const char* ifile, LC_ButtonActionType t, int a) {
    if(bg_image[t]) {
        SDL_DestroyTexture(bg_image[t]);
        bg_image[t] = NULL;
    }
    bg_image[t] = LC_Utils::LC_LoadTexture(ir,ifile,0,0,0,a);
}

void LC_Button::setImage(SDL_Renderer* ir, const char* ifile, int a) {
    setImage(ir,ifile,LC_BUTTON_ACTION_TYPE_UP,a);
    setImage(ir,ifile,LC_BUTTON_ACTION_TYPE_OVER,a);
    setImage(ir,ifile,LC_BUTTON_ACTION_TYPE_DOWN,a);
}
