#include "LC_LivingColour.h"

#include <iostream>
using namespace std;

#include <SDL.h>

#include "LC_GamePanel.h"
#include "LC_MenuBar.h"
#include "LC_Environment.h"
//#include "LC_MouseState.h"

#include <cmath>

LC_LivingColour::LC_LivingColour() : LC_GamePanel() {
    screen_width = LC_INIT_SCREEN_WIDTH;
    screen_height = LC_INIT_SCREEN_HEIGHT;
    full_screen = LC_INIT_FULL_SCREEN;
}

LC_LivingColour::LC_LivingColour(int in_max_buttons, int in_max_gamepanels) : LC_GamePanel(in_max_buttons, in_max_gamepanels) {
    LC_LivingColour();
}

LC_LivingColour::~LC_LivingColour() {
    ;
}

void LC_LivingColour::init() {
    if(!SDL_Init(SDL_INIT_VIDEO)) {
        window = SDL_CreateWindow("Living Colour", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screen_width, screen_height, full_screen&&SDL_WINDOW_FULLSCREEN);
        if(window) {
            screen_surface = SDL_GetWindowSurface(window);
            if(screen_surface) {
                renderer = SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED|SDL_RENDERER_PRESENTVSYNC);
                if(renderer) {
                    setRenderer(renderer);
                    if(SDL_SetRenderDrawBlendMode(renderer,SDL_BLENDMODE_BLEND)) cout << SDL_GetError() << endl;

                    // Set tool state defaults
                    tool_state.type = LC_TOOL_TYPE_CREATE;
                    tool_state.target = LC_TOOL_TARGET_TERRAIN;
                    tool_state.operation = LC_TOOL_OPERATION_FILL;
                    tool_state.shape = LC_TOOL_SHAPE_RECT;
                    tool_state.colour.r = tool_state.colour.g = tool_state.colour.b = tool_state.colour.a = 255;

                    // Create menu bar
                    menu_bar = new LC_MenuBar();
                    menu_bar->setRenderer(renderer);
                    menu_bar->init();

                    // Create environment
                    environment = new LC_Environment(screen_width/6,screen_height/6);
                    environment->setRenderer(renderer);
                    environment->screen_width = screen_width;
                    environment->screen_height = screen_height-menu_bar->screen_height;
                    environment->screen_y_pos = menu_bar->screen_height;

                    int this_w = environment->getWidth();
                    int this_h = environment->getHeight();
                    cout << "env dims = " << this_w << " x " << this_h << endl;
                    environment->refreshIDList();
                    //environment->fillTerrain(255,255,255,this_w/2,this_h/2,this_w/4,this_h/4);
                    //environment->spawnLives(0.1);

                    addGamePanel(environment);
                    addGamePanel(menu_bar);
                } else {
                    cout << SDL_GetError() << endl;
                }
            } else {
                cout << SDL_GetError() << endl;
            }
        } else {
            cout << SDL_GetError() << endl;
        }
    } else {
        cout << SDL_GetError() << endl;
    }
}

void LC_LivingColour::mainLoop() {
    while(!exit) {
        // Handle events
        SDL_Event this_event;
        while(SDL_PollEvent(&this_event)) {
            switch(this_event.type) {
                case SDL_WINDOWEVENT:
                    windowUpdate(&this_event);
                    break;
                case SDL_KEYDOWN:
                case SDL_KEYUP:
                    keyUpdate(&this_event);
                    break;
                case SDL_MOUSEMOTION:
                case SDL_MOUSEBUTTONDOWN:
                case SDL_MOUSEBUTTONUP:
                    mouseUpdate(&this_event);
                    break;
            }
        }
        frameUpdate();
        render();
    }
}

void LC_LivingColour::deinit() {
    // Destroy environment
    // Destroy menu bar
    menu_bar = NULL;
    if(renderer) SDL_DestroyRenderer(renderer);
    renderer = NULL;
    if(window) SDL_DestroyWindow(window);
    window = NULL;
    screen_surface = NULL;
    SDL_Quit();
}

void LC_LivingColour::windowUpdate(SDL_Event* event) {
    if(event->type==SDL_WINDOWEVENT){
        if(event->window.event==SDL_WINDOWEVENT_CLOSE) {
            exit = true;
        }
    }

    LC_GamePanel::windowUpdate(event);
}

void LC_LivingColour::keyUpdate(SDL_Event* event) {
    if(event->type==SDL_KEYDOWN) {
        switch(event->key.keysym.scancode) {
            case SDL_SCANCODE_F:
                toggleFullscreen();
                break;
            default:
                break;
        }
    }
    LC_GamePanel::keyUpdate(event);
}

bool LC_LivingColour::mouseUpdate(SDL_Event* event) {
    switch(event->type) {
        case SDL_MOUSEMOTION:
            if(left_mouse_down) {
                if(event->motion.x<left_mouse_down_x) {
                    tool_state.rect.x = event->motion.x;
                } else {
                    tool_state.rect.x = left_mouse_down_x;
                }
                if(event->motion.y<left_mouse_down_y) {
                    tool_state.rect.y = event->motion.y;
                } else {
                    tool_state.rect.y = left_mouse_down_y;
                }
                tool_state.rect.w = fabs(event->motion.x - left_mouse_down_x);
                tool_state.rect.h = fabs(event->motion.y - left_mouse_down_y);
            }
            break;
        case SDL_MOUSEBUTTONDOWN:
            if(event->button.button == SDL_BUTTON_LEFT) {
                left_mouse_down = 1;
                left_mouse_down_x = event->motion.x;
                left_mouse_down_y = event->motion.y;
                tool_state.rect.x = left_mouse_down_x;
                tool_state.rect.y = left_mouse_down_y;
            }
            break;
        case SDL_MOUSEBUTTONUP:
            if(event->button.button == SDL_BUTTON_LEFT) {
                left_mouse_down = 0;
                if(event->button.x<left_mouse_down_x) {
                    tool_state.rect.x = event->button.x;
                } else {
                    tool_state.rect.x = left_mouse_down_x;
                }
                if(event->button.y<left_mouse_down_y) {
                    tool_state.rect.y = event->button.y;
                } else {
                    tool_state.rect.y = left_mouse_down_y;
                }
                tool_state.rect.w = fabs(event->button.x - left_mouse_down_x);
                tool_state.rect.h = fabs(event->button.y - left_mouse_down_y);

                // Do something
                environment->coordsMouseToScreen(&tool_state.rect.x,&tool_state.rect.y,&tool_state.rect.w,&tool_state.rect.h);
                double this_x = tool_state.rect.x;
                double this_y = tool_state.rect.y;
                double this_w = tool_state.rect.w;
                double this_h = tool_state.rect.h;
                environment->coordsScreenToWorld(&this_x,&this_y,&this_w,&this_h);
                this_x = round(this_x);
                this_y = round(this_y);
                this_w = round(this_w);
                this_h = round(this_h);

                switch(tool_state.type) {
                    case LC_TOOL_TYPE_CREATE:
                        {
                            switch(tool_state.target) {
                                case LC_TOOL_TARGET_LIFE:
                                    {
                                        switch(tool_state.shape){
                                            case LC_TOOL_SHAPE_RECT:
                                                {
                                                    switch(tool_state.operation) {
                                                        case LC_TOOL_OPERATION_FILL:
                                                            {
                                                                for(int i = this_x; i < (this_x+this_w); i++) {
                                                                    for(int j = this_y; j < (this_y+this_h); j++) {
                                                                        environment->spawnLife(tool_state.colour.r,tool_state.colour.g,tool_state.colour.b,GENDER_NULL,i, j);
                                                                    }
                                                                }
                                                                environment->refreshIDList();
                                                            }
                                                            break;
                                                    }
                                                }
                                                break;
                                        }
                                    }
                                    break;
                                case LC_TOOL_TARGET_TERRAIN:
                                    {
                                        switch(tool_state.shape){
                                            case LC_TOOL_SHAPE_RECT:
                                                {
                                                    switch(tool_state.operation) {
                                                        case LC_TOOL_OPERATION_FILL:
                                                            {
                                                                for(int i = this_x; i < (this_x+this_w); i++) {
                                                                    for(int j = this_y; j < (this_y+this_h); j++) {
                                                                        environment->spawnTerrain(tool_state.colour.r,tool_state.colour.g,tool_state.colour.b,i, j);
                                                                    }
                                                                }
                                                                environment->refreshIDList();
                                                            }
                                                            break;
                                                    }
                                                }
                                                break;
                                        }
                                    }
                                    break;
                            }
                        }
                        break;
                    case LC_TOOL_TYPE_ERASE:
                        {
                            switch(tool_state.target) {
                                case LC_TOOL_TARGET_LIFE:
                                    {
                                        switch(tool_state.shape){
                                            case LC_TOOL_SHAPE_RECT:
                                                {
                                                    switch(tool_state.operation) {
                                                        case LC_TOOL_OPERATION_FILL:
                                                            {
                                                                for(int i = this_x; i < (this_x+this_w); i++) {
                                                                    for(int j = this_y; j < (this_y+this_h); j++) {
                                                                        environment->eraseLife(i,j);
                                                                    }
                                                                }
                                                                environment->refreshIDList();
                                                            }
                                                            break;
                                                    }
                                                }
                                                break;
                                        }
                                    }
                                    break;
                                case LC_TOOL_TARGET_TERRAIN:
                                    {
                                        switch(tool_state.shape){
                                            case LC_TOOL_SHAPE_RECT:
                                                {
                                                    switch(tool_state.operation) {
                                                        case LC_TOOL_OPERATION_FILL:
                                                            {
                                                                for(int i = this_x; i < (this_x+this_w); i++) {
                                                                    for(int j = this_y; j < (this_y+this_h); j++) {
                                                                        environment->eraseLife(i,j);
                                                                        environment->spawnTerrain(-1,-1,-1,i, j);
                                                                    }
                                                                }
                                                                environment->refreshIDList();
                                                            }

                                                            break;
                                                    }
                                                }
                                                break;
                                        }
                                    }
                                    break;
                            }
                        }
                        break;
                }
                //environment->fillTerrain(tool_state.colour.r,tool_state.colour.g,tool_state.colour.b,this_w,this_h,this_x,this_y);

                tool_state.rect.x = 0;
                tool_state.rect.y = 0;
                tool_state.rect.w = 0;
                tool_state.rect.h = 0;
            }
            break;
    }
    return LC_GamePanel::mouseUpdate(event);
}

void LC_LivingColour::frameUpdate() {
    LC_GamePanel::frameUpdate();
}

void LC_LivingColour::render() {
    SDL_SetRenderDrawColor(renderer,0,0,0,255);
    SDL_RenderClear(renderer);

    LC_GamePanel::render();

    // Render tool
    switch(tool_state.shape) {
        case LC_TOOL_SHAPE_RECT:
            if(left_mouse_down) {
                //cout << "x y w h = " << tool_state.rect.x << " " << tool_state.rect.y << " " << tool_state.rect.w << " " << tool_state.rect.h << " " << endl;
                SDL_SetRenderDrawColor(renderer,255,0,0,255);
                SDL_RenderDrawRect(renderer,&tool_state.rect);
            }
            break;
    }


    SDL_RenderPresent(renderer);
}

bool LC_LivingColour::isLeftMouseDown() const {
    return left_mouse_down;
}

int LC_LivingColour::getLeftMouseDownX() const {
    return left_mouse_down_x;
}

int LC_LivingColour::getLeftMouseDownY() const {
    return left_mouse_down_y;
}

void LC_LivingColour::resetDisplay() {
    // Needs work to make textures update too
    if(renderer) SDL_DestroyRenderer(renderer);
    renderer = NULL;
    if(window) SDL_DestroyWindow(window);
    window = NULL;
    window = SDL_CreateWindow("Living Colour", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screen_width, screen_height, full_screen&&SDL_WINDOW_FULLSCREEN);
    if(window) {
        screen_surface = SDL_GetWindowSurface(window);
        if(screen_surface) {
            renderer = SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED|SDL_RENDERER_PRESENTVSYNC);
            if(renderer) {
                setRenderer(renderer);
                if(SDL_SetRenderDrawBlendMode(renderer,SDL_BLENDMODE_BLEND)) cout << SDL_GetError() << endl;
                for(int i = 0; i < n_gamepanels; i++) {
                    if(gamepanels[i]) gamepanels[i]->setRenderer(renderer);
                }
            } else {
                cout << SDL_GetError() << endl;
            }
        } else {
            cout << SDL_GetError() << endl;
        }
    } else {
        cout << SDL_GetError() << endl;
    }
}

void LC_LivingColour::toggleFullscreen() {
    full_screen = !full_screen;
    resetDisplay();
}
