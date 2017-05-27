#include "LC_MenuBar.h"

#include <iostream>
using namespace std;

#include <SDL.h>

#include "LC_GamePanel.h"
#include "LC_ButtonGroup.h"

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
        screen_width = parent->screen_width;//LC_GetScreenWidth();
        screen_height = parent->screen_height*0.05;
        world_width = this_height*screen_width/screen_height;
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
        this_button->setColour(150,174,205,255,LC_CLICKABLE_STATE_UP);
        this_button->setColour(128,128,128,255,LC_CLICKABLE_STATE_OVER);
        this_button->setColour(64,64,64,255,LC_CLICKABLE_STATE_DOWN);
        this_button->setImage(LC_MENUBAR_STOPBUTTON_IMAGE,255);
        this_button->mouseDownAction = LC_Pause;
        addButton(this_button);
    }
    // Create play button
    {
        LC_Button* this_button = new LC_Button;
        this_button->setGeometry(8,1,6,6);
        this_button->setColour(150,174,205,255,LC_CLICKABLE_STATE_UP);
        this_button->setColour(128,128,128,255,LC_CLICKABLE_STATE_OVER);
        this_button->setColour(64,64,64,255,LC_CLICKABLE_STATE_DOWN);
        this_button->setImage(LC_MENUBAR_PLAYBUTTON_IMAGE,255);
        this_button->mouseDownAction = LC_Play;
        addButton(this_button);
    }
    // Create step button
    {
        LC_Button* this_button = new LC_Button;
        this_button->setGeometry(15,1,6,6);
        this_button->setColour(150,174,205,255,LC_CLICKABLE_STATE_UP);
        this_button->setColour(128,128,128,255,LC_CLICKABLE_STATE_OVER);
        this_button->setColour(64,64,64,255,LC_CLICKABLE_STATE_DOWN);
        this_button->setImage(LC_MENUBAR_STEPBUTTON_IMAGE,255);
        this_button->mouseDownAction = LC_Step;
        addButton(this_button);
    }
    // Create slower button
    {
        LC_Button* this_button = new LC_Button;
        this_button->setGeometry(25,1,6,6);
        this_button->setColour(150,174,205,255,LC_CLICKABLE_STATE_UP);
        this_button->setColour(128,128,128,255,LC_CLICKABLE_STATE_OVER);
        this_button->setColour(64,64,64,255,LC_CLICKABLE_STATE_DOWN);
        this_button->setImage("./res/slower.bmp",255);
        this_button->mouseDownAction = LC_IncrementPeriod;
        addButton(this_button);
    }
    // Create faster button
    {
        LC_Button* this_button = new LC_Button;
        this_button->setGeometry(32,1,6,6);
        this_button->setColour(150,174,205,255,LC_CLICKABLE_STATE_UP);
        this_button->setColour(128,128,128,255,LC_CLICKABLE_STATE_OVER);
        this_button->setColour(64,64,64,255,LC_CLICKABLE_STATE_DOWN);
        this_button->setImage("./res/faster.bmp",255);
        this_button->mouseDownAction = LC_DecrementPeriod;
        addButton(this_button);
    }
    /*// Colour buttons
    // Decrement R
    {
        LC_Button* this_button = new LC_Button;
        this_button->setGeometry(120,1,6,2);
        this_button->setColour(0,255,255,255,LC_CLICKABLE_STATE_UP);
        this_button->setColour(128,128,128,255,LC_CLICKABLE_STATE_OVER);
        this_button->setColour(64,64,64,255,LC_CLICKABLE_STATE_DOWN);
        this_button->setImage("./res/button_up.bmp",255);
        this_button->mouseDownAction = LC_DecrementR;
        addButton(this_button);
    }
    // Decrement G
    {
        LC_Button* this_button = new LC_Button;
        this_button->setGeometry(120,3,6,2);
        this_button->setColour(255,0,255,255,LC_CLICKABLE_STATE_UP);
        this_button->setColour(128,128,128,255,LC_CLICKABLE_STATE_OVER);
        this_button->setColour(64,64,64,255,LC_CLICKABLE_STATE_DOWN);
        this_button->setImage("./res/button_up.bmp",255);
        this_button->mouseDownAction = LC_DecrementG;
        addButton(this_button);
    }
    // Decrement B
    {
        LC_Button* this_button = new LC_Button;
        this_button->setGeometry(120,5,6,2);
        this_button->setColour(255,255,0,255,LC_CLICKABLE_STATE_UP);
        this_button->setColour(128,128,128,255,LC_CLICKABLE_STATE_OVER);
        this_button->setColour(64,64,64,255,LC_CLICKABLE_STATE_DOWN);
        this_button->setImage("./res/button_up.bmp",255);
        this_button->mouseDownAction = LC_DecrementB;
        addButton(this_button);
    }
    // Colour buttons
    // Increment R
    {
        LC_Button* this_button = new LC_Button;
        this_button->setGeometry(134,1,6,2);
        this_button->setColour(255,0,0,255,LC_CLICKABLE_STATE_UP);
        this_button->setColour(128,128,128,255,LC_CLICKABLE_STATE_OVER);
        this_button->setColour(64,64,64,255,LC_CLICKABLE_STATE_DOWN);
        this_button->setImage("./res/button_up.bmp",255);
        this_button->mouseDownAction = LC_IncrementR;
        addButton(this_button);
    }
    // Increment G
    {
        LC_Button* this_button = new LC_Button;
        this_button->setGeometry(134,3,6,2);
        this_button->setColour(0,255,0,255,LC_CLICKABLE_STATE_UP);
        this_button->setColour(128,128,128,255,LC_CLICKABLE_STATE_OVER);
        this_button->setColour(64,64,64,255,LC_CLICKABLE_STATE_DOWN);
        this_button->setImage("./res/button_up.bmp",255);
        this_button->mouseDownAction = LC_IncrementG;
        addButton(this_button);
    }
    // Increment B
    {
        LC_Button* this_button = new LC_Button;
        this_button->setGeometry(134,5,6,2);
        this_button->setColour(0,0,255,255,LC_CLICKABLE_STATE_UP);
        this_button->setColour(128,128,128,255,LC_CLICKABLE_STATE_OVER);
        this_button->setColour(64,64,64,255,LC_CLICKABLE_STATE_DOWN);
        this_button->setImage("./res/button_up.bmp",255);
        this_button->mouseDownAction = LC_IncrementB;
        addButton(this_button);
    }*/
    // Set R
    {
        LC_Button* this_button = new LC_Button;
        this_button->setGeometry(134,1,6,6);
        this_button->setColour(255,0,0,255,LC_CLICKABLE_STATE_UP);
        this_button->setColour(128,128,128,255,LC_CLICKABLE_STATE_OVER);
        this_button->setColour(64,64,64,255,LC_CLICKABLE_STATE_DOWN);
        this_button->setImage("./res/button_up.bmp",255);
        this_button->mouseDownAction = LC_SetR;
        addButton(this_button);
    }
    // Set G
    {
        LC_Button* this_button = new LC_Button;
        this_button->setGeometry(141,1,6,6);
        this_button->setColour(0,255,0,255,LC_CLICKABLE_STATE_UP);
        this_button->setColour(128,128,128,255,LC_CLICKABLE_STATE_OVER);
        this_button->setColour(64,64,64,255,LC_CLICKABLE_STATE_DOWN);
        this_button->setImage("./res/button_up.bmp",255);
        this_button->mouseDownAction = LC_SetG;
        addButton(this_button);
    }
    // Set B
    {
        LC_Button* this_button = new LC_Button;
        this_button->setGeometry(148,1,6,6);
        this_button->setColour(0,0,255,255,LC_CLICKABLE_STATE_UP);
        this_button->setColour(128,128,128,255,LC_CLICKABLE_STATE_OVER);
        this_button->setColour(64,64,64,255,LC_CLICKABLE_STATE_DOWN);
        this_button->setImage("./res/button_up.bmp",255);
        this_button->mouseDownAction = LC_SetB;
        addButton(this_button);
    }
    // Set M
    {
        LC_Button* this_button = new LC_Button;
        this_button->setGeometry(155,1,6,6);
        this_button->setColour(255,0,255,255,LC_CLICKABLE_STATE_UP);
        this_button->setColour(128,128,128,255,LC_CLICKABLE_STATE_OVER);
        this_button->setColour(64,64,64,255,LC_CLICKABLE_STATE_DOWN);
        this_button->setImage("./res/button_up.bmp",255);
        this_button->mouseDownAction = LC_SetM;
        addButton(this_button);
    }
    // Set C
    {
        LC_Button* this_button = new LC_Button;
        this_button->setGeometry(162,1,6,6);
        this_button->setColour(0,255,255,255,LC_CLICKABLE_STATE_UP);
        this_button->setColour(128,128,128,255,LC_CLICKABLE_STATE_OVER);
        this_button->setColour(64,64,64,255,LC_CLICKABLE_STATE_DOWN);
        this_button->setImage("./res/button_up.bmp",255);
        this_button->mouseDownAction = LC_SetC;
        addButton(this_button);
    }
    // Set Y
    {
        LC_Button* this_button = new LC_Button;
        this_button->setGeometry(169,1,6,6);
        this_button->setColour(255,255,0,255,LC_CLICKABLE_STATE_UP);
        this_button->setColour(128,128,128,255,LC_CLICKABLE_STATE_OVER);
        this_button->setColour(64,64,64,255,LC_CLICKABLE_STATE_DOWN);
        this_button->setImage("./res/button_up.bmp",255);
        this_button->mouseDownAction = LC_SetY;
        addButton(this_button);
    }
    // Set W
    {
        LC_Button* this_button = new LC_Button;
        this_button->setGeometry(176,1,6,6);
        this_button->setColour(255,255,255,255,LC_CLICKABLE_STATE_UP);
        this_button->setColour(128,128,128,255,LC_CLICKABLE_STATE_OVER);
        this_button->setColour(64,64,64,255,LC_CLICKABLE_STATE_DOWN);
        this_button->setImage("./res/button_up.bmp",255);
        this_button->mouseDownAction = LC_SetW;
        addButton(this_button);
    }
    // Set K
    {
        LC_Button* this_button = new LC_Button;
        this_button->setGeometry(183,1,6,6);
        this_button->setColour(0,0,0,255,LC_CLICKABLE_STATE_UP);
        this_button->setColour(128,128,128,255,LC_CLICKABLE_STATE_OVER);
        this_button->setColour(64,64,64,255,LC_CLICKABLE_STATE_DOWN);
        this_button->setImage("./res/button_up.bmp",255);
        this_button->mouseDownAction = LC_SetK;
        addButton(this_button);
    }
    // Set H
    {
        LC_Button* this_button = new LC_Button;
        this_button->setGeometry(190,1,6,6);
        this_button->setColour(128,128,128,255,LC_CLICKABLE_STATE_UP);
        this_button->setColour(128,128,128,255,LC_CLICKABLE_STATE_OVER);
        this_button->setColour(64,64,64,255,LC_CLICKABLE_STATE_DOWN);
        this_button->setImage("./res/button_up.bmp",255);
        this_button->mouseDownAction = LC_SetH;
        addButton(this_button);
    }
    // Type button group
    {
        LC_ButtonGroup* this_button_group = new LC_ButtonGroup(3,0);
        this_button_group->screen_x_pos = 44*screen_width/world_width;
        this_button_group->screen_y_pos = 0;
        this_button_group->screen_width = 21*screen_width/world_width;
        this_button_group->screen_height = screen_height;
        this_button_group->world_width = 21;
        this_button_group->bg_colour = new SDL_Color();
        this_button_group->bg_colour->r = 255;
        this_button_group->bg_colour->g = 255;
        this_button_group->bg_colour->b = 255;
        this_button_group->bg_colour->a = 0;
        // Select
        {
            LC_Button* this_button = new LC_Button;
            this_button->setGeometry(0,1,6,6);
            this_button->setColour(150,174,205,255,LC_CLICKABLE_STATE_UP);
            this_button->setColour(128,128,128,255,LC_CLICKABLE_STATE_OVER);
            this_button->setColour(64,64,64,255,LC_CLICKABLE_STATE_DOWN);
            this_button->setImage("./res/select.bmp",255);
            this_button->mouseDownAction = LC_setToolTypeSelect;
            this_button_group->addButton(this_button);
        }
        // Create
        {
            LC_Button* this_button = new LC_Button;
            this_button->setGeometry(7,1,6,6);
            this_button->setColour(150,174,205,255,LC_CLICKABLE_STATE_UP);
            this_button->setColour(128,128,128,255,LC_CLICKABLE_STATE_OVER);
            this_button->setColour(64,64,64,255,LC_CLICKABLE_STATE_DOWN);
            this_button->setImage("./res/create.bmp",255);
            this_button->mouseDownAction = LC_setToolTypeCreate;
            this_button_group->addButton(this_button);
        }
        // Erase
        {
            LC_Button* this_button = new LC_Button;
            this_button->setGeometry(14,1,6,6);
            this_button->setColour(150,174,205,255,LC_CLICKABLE_STATE_UP);
            this_button->setColour(128,128,128,255,LC_CLICKABLE_STATE_OVER);
            this_button->setColour(64,64,64,255,LC_CLICKABLE_STATE_DOWN);
            this_button->setImage("./res/erase.bmp",255);
            this_button->mouseDownAction = LC_setToolTypeErase;
            this_button_group->addButton(this_button);
        }
        addGamePanel(this_button_group);
    }
    // Target button group
    {
        LC_ButtonGroup* this_button_group = new LC_ButtonGroup(2,0);
        this_button_group->screen_x_pos = 70*screen_width/world_width;
        this_button_group->screen_y_pos = 0;
        this_button_group->screen_width = 14*screen_width/world_width;
        this_button_group->screen_height = screen_height;
        this_button_group->world_width = 14;
        this_button_group->bg_colour = new SDL_Color();
        this_button_group->bg_colour->r = 255;
        this_button_group->bg_colour->g = 255;
        this_button_group->bg_colour->b = 255;
        this_button_group->bg_colour->a = 0;

        // Life
        {
            LC_Button* this_button = new LC_Button;
            this_button->setGeometry(0,1,6,6);
            this_button->setColour(150,174,205,255,LC_CLICKABLE_STATE_UP);
            this_button->setColour(128,128,128,255,LC_CLICKABLE_STATE_OVER);
            this_button->setColour(64,64,64,255,LC_CLICKABLE_STATE_DOWN);
            this_button->setImage("./res/life.bmp",255);
            this_button->mouseDownAction = LC_setToolTargetLife;
            this_button_group->addButton(this_button);
        }
        // Terrain
        {
            LC_Button* this_button = new LC_Button;
            this_button->setGeometry(7,1,6,6);
            this_button->setColour(150,174,205,255,LC_CLICKABLE_STATE_UP);
            this_button->setColour(128,128,128,255,LC_CLICKABLE_STATE_OVER);
            this_button->setColour(64,64,64,255,LC_CLICKABLE_STATE_DOWN);
            this_button->setImage("./res/terrain.bmp",255);
            this_button->mouseDownAction = LC_setToolTargetTerrain;
            this_button_group->addButton(this_button);
        }
        addGamePanel(this_button_group);
    }
    // Operation button group
    {
        LC_ButtonGroup* this_button_group = new LC_ButtonGroup(3,0);
        this_button_group->screen_x_pos = 89*screen_width/world_width;
        this_button_group->screen_y_pos = 0;
        this_button_group->screen_width = 21*screen_width/world_width;
        this_button_group->screen_height = screen_height;
        this_button_group->world_width = 21;
        this_button_group->bg_colour = new SDL_Color();
        this_button_group->bg_colour->r = 255;
        this_button_group->bg_colour->g = 255;
        this_button_group->bg_colour->b = 255;
        this_button_group->bg_colour->a = 0;

        // Draw
        {
            LC_Button* this_button = new LC_Button;
            this_button->setGeometry(0,1,6,6);
            this_button->setColour(150,174,205,255,LC_CLICKABLE_STATE_UP);
            this_button->setColour(128,128,128,255,LC_CLICKABLE_STATE_OVER);
            this_button->setColour(64,64,64,255,LC_CLICKABLE_STATE_DOWN);
            this_button->setImage("./res/draw.bmp",255);
            this_button->mouseDownAction = LC_setToolOperationDraw;
            this_button_group->addButton(this_button);
        }
        // Fill
        {
            LC_Button* this_button = new LC_Button;
            this_button->setGeometry(7,1,6,6);
            this_button->setColour(150,174,205,255,LC_CLICKABLE_STATE_UP);
            this_button->setColour(128,128,128,255,LC_CLICKABLE_STATE_OVER);
            this_button->setColour(64,64,64,255,LC_CLICKABLE_STATE_DOWN);
            this_button->setImage("./res/fill.bmp",255);
            this_button->mouseDownAction = LC_setToolOperationFill;
            this_button_group->addButton(this_button);
        }
        // Spray
        {
            LC_Button* this_button = new LC_Button;
            this_button->setGeometry(14,1,6,6);
            this_button->setColour(150,174,205,255,LC_CLICKABLE_STATE_UP);
            this_button->setColour(128,128,128,255,LC_CLICKABLE_STATE_OVER);
            this_button->setColour(64,64,64,255,LC_CLICKABLE_STATE_DOWN);
            this_button->setImage("./res/spray.bmp",255);
            this_button->mouseDownAction = LC_setToolOperationSpray;
            this_button_group->addButton(this_button);
        }
        addGamePanel(this_button_group);
    }
    // Shape button group
    {
        LC_ButtonGroup* this_button_group = new LC_ButtonGroup(2,0);
        this_button_group->screen_x_pos = 115*screen_width/world_width;
        this_button_group->screen_y_pos = 0;
        this_button_group->screen_width = 14*screen_width/world_width;
        this_button_group->screen_height = screen_height;
        this_button_group->world_width = 14;
        this_button_group->bg_colour = new SDL_Color();
        this_button_group->bg_colour->r = 255;
        this_button_group->bg_colour->g = 255;
        this_button_group->bg_colour->b = 255;
        this_button_group->bg_colour->a = 0;

        // Rect
        {
            LC_Button* this_button = new LC_Button;
            this_button->setGeometry(0,1,6,6);
            this_button->setColour(150,174,205,255,LC_CLICKABLE_STATE_UP);
            this_button->setColour(128,128,128,255,LC_CLICKABLE_STATE_OVER);
            this_button->setColour(64,64,64,255,LC_CLICKABLE_STATE_DOWN);
            this_button->setImage("./res/rect.bmp",255);
            this_button->mouseDownAction = LC_setToolShapeRect;
            this_button_group->addButton(this_button);
        }
        // Circle
        {
            LC_Button* this_button = new LC_Button;
            this_button->setGeometry(7,1,6,6);
            this_button->setColour(150,174,205,255,LC_CLICKABLE_STATE_UP);
            this_button->setColour(128,128,128,255,LC_CLICKABLE_STATE_OVER);
            this_button->setColour(64,64,64,255,LC_CLICKABLE_STATE_DOWN);
            this_button->setImage("./res/circle.bmp",255);
            this_button->mouseDownAction = LC_setToolShapeCircle;
            this_button_group->addButton(this_button);
        }
        addGamePanel(this_button_group);
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
