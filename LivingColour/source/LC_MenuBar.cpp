#include "LC_MenuBar.h"

#include <iostream>
using namespace std;

#include <SDL.h>

#include "LC_GamePanel.h"

#include "LC_LivingColour.h"

LC_MenuBar::LC_MenuBar() {
    ;
}

LC_MenuBar::~LC_MenuBar() {
    ;
}

void LC_MenuBar::init() {
    // Set size
    {
        double this_height = 8.0;
        screen_width = LC_GetScreenWidth();
        screen_height = LC_GetScreenHeight()*0.05;
        world_width = this_height*screen_width/(double)screen_height;
    }
    // Set background colour
    {
        bg_colour = new SDL_Color();
        bg_colour->r = 200;
        bg_colour->g = 224;
        bg_colour->b = 240;
        bg_colour->a = 255;
    }
    // Create stop button
    {
        LC_Button* this_button = new LC_Button;
        this_button->setGeometry(1,1,6,6);
        this_button->setColour(150,174,205,255,LC_BUTTON_ACTION_TYPE_UP);
        this_button->setColour(128,128,128,255,LC_BUTTON_ACTION_TYPE_OVER);
        this_button->setColour(64,64,64,255,LC_BUTTON_ACTION_TYPE_DOWN);
        if(renderer) this_button->setImage(renderer,LC_MENUBAR_STOPBUTTON_IMAGE,255);
        this_button->mouseDownAction = LC_Pause;
        addButton(this_button);
    }
    // Create play button
    {
        LC_Button* this_button = new LC_Button;
        this_button->setGeometry(8,1,6,6);
        this_button->setColour(150,174,205,255,LC_BUTTON_ACTION_TYPE_UP);
        this_button->setColour(128,128,128,255,LC_BUTTON_ACTION_TYPE_OVER);
        this_button->setColour(64,64,64,255,LC_BUTTON_ACTION_TYPE_DOWN);
        if(renderer) this_button->setImage(renderer,LC_MENUBAR_PLAYBUTTON_IMAGE,255);
        this_button->mouseDownAction = LC_Play;
        addButton(this_button);
    }
    // Create step button
    {
        LC_Button* this_button = new LC_Button;
        this_button->setGeometry(15,1,6,6);
        this_button->setColour(150,174,205,255,LC_BUTTON_ACTION_TYPE_UP);
        this_button->setColour(128,128,128,255,LC_BUTTON_ACTION_TYPE_OVER);
        this_button->setColour(64,64,64,255,LC_BUTTON_ACTION_TYPE_DOWN);
        if(renderer) this_button->setImage(renderer,LC_MENUBAR_STEPBUTTON_IMAGE,255);
        this_button->mouseDownAction = LC_Step;
        addButton(this_button);
    }
    // Create slower button
    {
        LC_Button* this_button = new LC_Button;
        this_button->setGeometry(25,1,6,6);
        this_button->setColour(150,174,205,255,LC_BUTTON_ACTION_TYPE_UP);
        this_button->setColour(128,128,128,255,LC_BUTTON_ACTION_TYPE_OVER);
        this_button->setColour(64,64,64,255,LC_BUTTON_ACTION_TYPE_DOWN);
        if(renderer) this_button->setImage(renderer,"slower.bmp",255);
        this_button->mouseDownAction = LC_IncrementPeriod;
        addButton(this_button);
    }
    // Create faster button
    {
        LC_Button* this_button = new LC_Button;
        this_button->setGeometry(32,1,6,6);
        this_button->setColour(150,174,205,255,LC_BUTTON_ACTION_TYPE_UP);
        this_button->setColour(128,128,128,255,LC_BUTTON_ACTION_TYPE_OVER);
        this_button->setColour(64,64,64,255,LC_BUTTON_ACTION_TYPE_DOWN);
        if(renderer) this_button->setImage(renderer,"faster.bmp",255);
        this_button->mouseDownAction = LC_DecrementPeriod;
        addButton(this_button);
    }
    // Create tool 1 button
    {
        LC_Button* this_button = new LC_Button;
        this_button->setGeometry(world_width-28,1,6,6);
        this_button->setColour(150,174,205,255,LC_BUTTON_ACTION_TYPE_UP);
        this_button->setColour(128,128,128,255,LC_BUTTON_ACTION_TYPE_OVER);
        this_button->setColour(64,64,64,255,LC_BUTTON_ACTION_TYPE_DOWN);
        if(renderer) this_button->setImage(renderer,"createterrain.bmp",255);
        this_button->mouseDownAction = LC_SelectTool1;
        addButton(this_button);
    }
    // Create tool 2 button
    {
        LC_Button* this_button = new LC_Button;
        this_button->setGeometry(world_width-21,1,6,6);
        this_button->setColour(150,174,205,255,LC_BUTTON_ACTION_TYPE_UP);
        this_button->setColour(128,128,128,255,LC_BUTTON_ACTION_TYPE_OVER);
        this_button->setColour(64,64,64,255,LC_BUTTON_ACTION_TYPE_DOWN);
        if(renderer) this_button->setImage(renderer,"eraseterrain.bmp",255);
        this_button->mouseDownAction = LC_SelectTool2;
        addButton(this_button);
    }
    // Create tool 3 button
    {
        LC_Button* this_button = new LC_Button;
        this_button->setGeometry(world_width-14,1,6,6);
        this_button->setColour(150,174,205,255,LC_BUTTON_ACTION_TYPE_UP);
        this_button->setColour(128,128,128,255,LC_BUTTON_ACTION_TYPE_OVER);
        this_button->setColour(64,64,64,255,LC_BUTTON_ACTION_TYPE_DOWN);
        if(renderer) this_button->setImage(renderer,"createherb.bmp",255);
        this_button->mouseDownAction = LC_SelectTool3;
        addButton(this_button);
    }
    // Create tool 4 button
    {
        LC_Button* this_button = new LC_Button;
        this_button->setGeometry(world_width-7,1,6,6);
        this_button->setColour(150,174,205,255,LC_BUTTON_ACTION_TYPE_UP);
        this_button->setColour(128,128,128,255,LC_BUTTON_ACTION_TYPE_OVER);
        this_button->setColour(64,64,64,255,LC_BUTTON_ACTION_TYPE_DOWN);
        if(renderer) this_button->setImage(renderer,"eraseherb.bmp",255);
        this_button->mouseDownAction = LC_SelectTool4;
        addButton(this_button);
    }
}

void LC_MenuBar::windowUpdate(SDL_Event* event) {
    LC_GamePanel::windowUpdate(event);
}

void LC_MenuBar::keyUpdate(SDL_Event* event) {
    LC_GamePanel::keyUpdate(event);
}

bool LC_MenuBar::mouseUpdate(SDL_Event* event) {
    return LC_GamePanel::mouseUpdate(event);
}

void LC_MenuBar::frameUpdate() {
    LC_GamePanel::frameUpdate();
}

void LC_MenuBar::render() {
    LC_GamePanel::render();
}
