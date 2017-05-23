#include "LC_Button.h"

#include <iostream>
using namespace std;

#include <SDL.h>

#include "LC_Utils.h"
#include "LC_RenderObject.h"
#include "LC_Clickable.h"
#include "LC_Image.h"

LC_Button::LC_Button() {
    /*
    bg_colour = new SDL_Colour*[LC_BUTTON_ACTION_TYPE_N];
    bg_image = new SDL_Texture*[LC_BUTTON_ACTION_TYPE_N];
    for(int i = 0; i < LC_BUTTON_ACTION_TYPE_N; i++) {
        bg_colour[i] = NULL;
        bg_image[i] = NULL;
    }
    */
    button_images = new LC_Image*[LC_CLICKABLE_STATE_N];
    for(int i = 0; i < LC_CLICKABLE_STATE_N; i++) {
        button_images[i] = NULL;
    }
}

LC_Button::~LC_Button() {
    /*
    for(int i = 0; i < LC_BUTTON_ACTION_TYPE_N; i++) {
        if(bg_colour[i]) delete bg_colour[i];
        if(bg_image[i]) delete bg_image;
    }
    delete [] bg_colour;
    delete [] bg_image;
    */
    for(int i = 0; i < LC_CLICKABLE_STATE_N; i++) {
        if(button_images[i]) delete button_images[i];
        button_images[i] = NULL;
    }
    delete [] button_images;
    button_images = NULL;
}

void LC_Button::setGeometry(int ix, int iy, int iw, int ih){
    /*
    x = ix;
    y = iy;
    w = iw;
    h = ih;
    */
    click_region.x = ix;
    click_region.y = iy;
    click_region.w = iw;
    click_region.h = ih;
}

void LC_Button::setColour(int r, int g, int b, int a, /*LC_ButtonActionType t*/LC_ClickableState t) {
    /*
    if(!bg_colour[t]) {
        bg_colour[t] = new SDL_Colour;
    }
    bg_colour[t]->r = r;
    bg_colour[t]->g = g;
    bg_colour[t]->b = b;
    bg_colour[t]->a = a;
    */
    if(!button_images[t]) {
        button_images[t] = new LC_Image();
        button_images[t]->setRenderer(renderer);
    }
    button_images[t]->bg_colour.r = r;
    button_images[t]->bg_colour.g = g;
    button_images[t]->bg_colour.b = b;
    button_images[t]->bg_colour.a = a;

}

void LC_Button::setColour(int r, int g, int b, int a) {
    for(int i = 0; i < LC_CLICKABLE_STATE_N; i++) {
        setColour(r,g,b,a);
    }
}

void LC_Button::setImage(/*SDL_Renderer* ir, */const char* ifile, /*LC_ButtonActionType t*/LC_ClickableState t, int a) {
    /*
    if(bg_image[t]) {
        SDL_DestroyTexture(bg_image[t]);
        bg_image[t] = NULL;
    }
    bg_image[t] = LC_Utils::LC_LoadTexture(ir,ifile,0,0,0,a);
    */
    if(!button_images[t]) {
        button_images[t] = new LC_Image();
        button_images[t]->setRenderer(renderer);
    }
    button_images[t]->texture_alpha = a;
    button_images[t]->loadImage(ifile);
}

void LC_Button::setRenderer(SDL_Renderer* ir) {
    LC_RenderObject::setRenderer(ir);
    //cout << "Button renderer set" << endl;
    for(int i = 0; i < LC_CLICKABLE_STATE_N; i++) {
        if(button_images[i]) {
            button_images[i]->setRenderer(ir);
        }
    }
}

void LC_Button::setImage(/*SDL_Renderer* ir, */const char* ifile, int a) {
    /*
    setImage(ir,ifile,LC_BUTTON_ACTION_TYPE_UP,a);
    setImage(ir,ifile,LC_BUTTON_ACTION_TYPE_OVER,a);
    setImage(ir,ifile,LC_BUTTON_ACTION_TYPE_DOWN,a);
    */
    setImage(ifile,LC_CLICKABLE_STATE_UP,a);
    setImage(ifile,LC_CLICKABLE_STATE_OVER,a);
    setImage(ifile,LC_CLICKABLE_STATE_DOWN,a);
}

void LC_Button::render() {
    if(renderer) {
        if(button_images[click_state]) {
            button_images[click_state]->image_region.x = click_region.x;
            button_images[click_state]->image_region.y = click_region.y;
            button_images[click_state]->image_region.w = click_region.w;
            button_images[click_state]->image_region.h = click_region.h;
            if(parent) {
                parent->coordsWorldToScreen(&button_images[click_state]->image_region.x,
                                &button_images[click_state]->image_region.y,
                                &button_images[click_state]->image_region.w,
                                &button_images[click_state]->image_region.h);
            }
            button_images[click_state]->render();
        }
    }
}
