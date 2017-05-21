#include "LC_Button.h"

#include <iostream>
using namespace std;

#include <SDL.h>

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
