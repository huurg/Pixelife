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
    world_width = screen_width;
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
                    menu_bar->setParent(this);
                    menu_bar->init();
                    menu_bg_color = menu_bar->bg_colour;
                    //cout << "Menu gamepanels = " << menu_bar->getN_GamePanels() << endl;

                    // Create environment
                    environment = new LC_Environment(screen_width/LC_INIT_ENVIRONMENT_SCALE,screen_height/LC_INIT_ENVIRONMENT_SCALE);
                    environment->setRenderer(renderer);
                    environment->screen_width = screen_width;
                    environment->screen_height = screen_height-menu_bar->screen_height;
                    environment->screen_y_pos = menu_bar->screen_height;

                    int this_w = environment->getWidth();
                    int this_h = environment->getHeight();
                    //cout << "env dims = " << this_w << " x " << this_h << endl;
                    environment->refreshIDList();
                    tool_state.rect.env_w = tool_state.circle.env_w = tool_state.diamond.env_w = tool_state.line.env_w = this_w;
                    tool_state.rect.env_h = tool_state.circle.env_h = tool_state.diamond.env_h = tool_state.line.env_h = this_h;
                    //environment->fillTerrain(255,255,255,this_w/2,this_h/2,this_w/4,this_h/4);
                    //environment->spawnLives(0.1);

                    addGamePanel(environment);
                    addGamePanel(menu_bar);

                    LC_setToolTypeSelect();
                    LC_setToolTargetLife();
                    LC_setToolOperationDraw();
                    LC_setToolShapeRect();
                    LC_SetW();
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
    //cout << endl;
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
    //cout << endl;
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
            case SDL_SCANCODE_ESCAPE:
                exit = true;
                break;
            case SDL_SCANCODE_LEFTBRACKET:
                tool_state.brush_size--;
                if(tool_state.brush_size<0) tool_state.brush_size = 0;
                tool_state.circle.circle_r_x = tool_state.brush_size;
                tool_state.circle.circle_r_y = tool_state.brush_size;
                tool_state.rect.rect_w = tool_state.brush_size;
                tool_state.rect.rect_h = tool_state.brush_size;
                break;
            case SDL_SCANCODE_RIGHTBRACKET:
                tool_state.brush_size++;
                tool_state.circle.circle_r_x = tool_state.brush_size;
                tool_state.circle.circle_r_y = tool_state.brush_size;
                tool_state.rect.rect_w = tool_state.brush_size;
                tool_state.rect.rect_h = tool_state.brush_size;
                break;
            case SDL_SCANCODE_Q:
                if(tool_state.colour.r<255) tool_state.colour.r++;
                break;
            case SDL_SCANCODE_W:
                if(tool_state.colour.g<255) tool_state.colour.g++;
                break;
            case SDL_SCANCODE_E:
                if(tool_state.colour.b<255) tool_state.colour.b++;
                break;
            case SDL_SCANCODE_A:
                if(tool_state.colour.r>0) tool_state.colour.r--;
                break;
            case SDL_SCANCODE_S:
                if(tool_state.colour.g>0) tool_state.colour.g--;
                break;
            case SDL_SCANCODE_D:
                if(tool_state.colour.b>0) tool_state.colour.b--;
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
            // Update tools
            // Rect
            if(tool_state.rect.centred) {
                tool_state.rect.rect_x = event->motion.x;
                tool_state.rect.rect_y = event->motion.y;
                tool_state.rect.rect_w = tool_state.brush_size;
                tool_state.rect.rect_h = tool_state.brush_size;
            } else {
                if(event->motion.x<left_mouse_down_x) {
                    tool_state.rect.rect_x = event->motion.x;
                } else {
                    tool_state.rect.rect_x = left_mouse_down_x;
                }
                if(event->motion.y<left_mouse_down_y) {
                    tool_state.rect.rect_y = event->motion.y;
                } else {
                    tool_state.rect.rect_y = left_mouse_down_y;
                }
                tool_state.rect.rect_w = fabs(event->motion.x - left_mouse_down_x);
                tool_state.rect.rect_h = fabs(event->motion.y - left_mouse_down_y);
            }
            // Circle
            if(tool_state.circle.centred) {
                tool_state.circle.circle_x = event->motion.x;
                tool_state.circle.circle_y = event->motion.y;
                tool_state.circle.circle_r_x = tool_state.brush_size;
                tool_state.circle.circle_r_y = tool_state.brush_size;
            } else {
                if(event->motion.x<left_mouse_down_x) {
                    tool_state.circle.circle_x = event->motion.x;
                } else {
                    tool_state.circle.circle_x = left_mouse_down_x;
                }
                if(event->motion.y<left_mouse_down_y) {
                    tool_state.circle.circle_y = event->motion.y;
                } else {
                    tool_state.circle.circle_y = left_mouse_down_y;
                }
                tool_state.circle.circle_r_x = fabs(event->motion.x - left_mouse_down_x)/2.0;
                tool_state.circle.circle_r_y = fabs(event->motion.y - left_mouse_down_y)/2.0;
            }
            // Spawn life if mouse is down and operation is draw
            if(LC_isLeftMouseDown()&&((tool_state.operation==LC_TOOL_OPERATION_DRAW)||(tool_state.operation==LC_TOOL_OPERATION_SPRAY))) {
                int r_x, r_y, r_w, r_h;

                r_x = tool_state.rect.rect_x;
                r_y = tool_state.rect.rect_y;
                r_w = tool_state.rect.rect_w;
                r_h = tool_state.rect.rect_h;

                environment->coordsMouseToScreen(&tool_state.rect.rect_x,&tool_state.rect.rect_y,&tool_state.rect.rect_w,&tool_state.rect.rect_h);
                environment->coordsScreenToWorld(&tool_state.rect.rect_x,&tool_state.rect.rect_y,&tool_state.rect.rect_w,&tool_state.rect.rect_h);

                int c_x, c_y, c_w, c_h;

                c_x = tool_state.circle.circle_x;
                c_y = tool_state.circle.circle_y;
                c_w = tool_state.circle.circle_r_x;
                c_h = tool_state.circle.circle_r_y;

                environment->coordsMouseToScreen(&tool_state.circle.circle_x,&tool_state.circle.circle_y,&tool_state.circle.circle_r_x,&tool_state.circle.circle_r_y);
                environment->coordsScreenToWorld(&tool_state.circle.circle_x,&tool_state.circle.circle_y,&tool_state.circle.circle_r_x,&tool_state.circle.circle_r_y);

                if(tool_state.operation==LC_TOOL_OPERATION_FILL) {
                    if(environment->isMouseOver()) environment->act(&tool_state);
                }

                if(environment->isMouseOver()) environment->act(&tool_state);

                tool_state.rect.rect_x = r_x;
                tool_state.rect.rect_y = r_y;
                tool_state.rect.rect_w = r_w;
                tool_state.rect.rect_h = r_h;
                tool_state.circle.circle_x = c_x;
                tool_state.circle.circle_y = c_y;
                tool_state.circle.circle_r_x = c_w;
                tool_state.circle.circle_r_y = c_h;
            }
            break;
        case SDL_MOUSEBUTTONDOWN:
            if(event->button.button == SDL_BUTTON_LEFT) {
                left_mouse_down = 1;
                left_mouse_down_x = event->button.x;
                left_mouse_down_y = event->button.y;
                // Update tools
                // Rect
                if(tool_state.rect.centred) {
                    tool_state.rect.rect_x = event->button.x;
                    tool_state.rect.rect_y = event->button.y;
                    tool_state.rect.rect_w = tool_state.brush_size;
                    tool_state.rect.rect_h = tool_state.brush_size;
                } else {
                    tool_state.rect.rect_x = left_mouse_down_x;
                    tool_state.rect.rect_y = left_mouse_down_y;
                    tool_state.rect.rect_w = 0;
                    tool_state.rect.rect_h = 0;
                }
                // Circle
                if(tool_state.circle.centred) {
                    tool_state.circle.circle_x = event->button.x;
                    tool_state.circle.circle_y = event->button.y;
                    tool_state.circle.circle_r_x = tool_state.brush_size;
                    tool_state.circle.circle_r_y = tool_state.brush_size;
                } else {
                    tool_state.circle.circle_x = event->button.x;
                    tool_state.circle.circle_y = event->button.y;
                    tool_state.circle.circle_r_x = 0;
                    tool_state.circle.circle_r_y = 0;
                }
            }

            // Print life at mouse position of type is select
            if(tool_state.type==LC_TOOL_TYPE_SELECT) {
                int this_mx = left_mouse_down_x;
                int this_my = left_mouse_down_y;
                int this_w = 0;
                int this_h = 0;
                environment->coordsMouseToScreen(&this_mx, &this_my,&this_w,&this_h);
                environment->coordsScreenToWorld(&this_mx, &this_my,&this_w,&this_h);
                //cout << this_mx << ", " << this_my << endl;
                environment->printLifeAt(this_mx,this_my);

            }

            // Spawn life if operation is draw
            if((tool_state.operation==LC_TOOL_OPERATION_DRAW)||(tool_state.operation==LC_TOOL_OPERATION_SPRAY)) {
                int r_x, r_y, r_w, r_h;

                r_x = tool_state.rect.rect_x;
                r_y = tool_state.rect.rect_y;
                r_w = tool_state.rect.rect_w;
                r_h = tool_state.rect.rect_h;

                environment->coordsMouseToScreen(&tool_state.rect.rect_x,&tool_state.rect.rect_y,&tool_state.rect.rect_w,&tool_state.rect.rect_h);
                environment->coordsScreenToWorld(&tool_state.rect.rect_x,&tool_state.rect.rect_y,&tool_state.rect.rect_w,&tool_state.rect.rect_h);

                int c_x, c_y, c_w, c_h;

                c_x = tool_state.circle.circle_x;
                c_y = tool_state.circle.circle_y;
                c_w = tool_state.circle.circle_r_x;
                c_h = tool_state.circle.circle_r_y;

                environment->coordsMouseToScreen(&tool_state.circle.circle_x,&tool_state.circle.circle_y,&tool_state.circle.circle_r_x,&tool_state.circle.circle_r_y);
                environment->coordsScreenToWorld(&tool_state.circle.circle_x,&tool_state.circle.circle_y,&tool_state.circle.circle_r_x,&tool_state.circle.circle_r_y);

                if(tool_state.operation==LC_TOOL_OPERATION_FILL) {
                    if(environment->isMouseOver()) environment->act(&tool_state);
                }

                if(environment->isMouseOver()) environment->act(&tool_state);

                tool_state.rect.rect_x = r_x;
                tool_state.rect.rect_y = r_y;
                tool_state.rect.rect_w = r_w;
                tool_state.rect.rect_h = r_h;
                tool_state.circle.circle_x = c_x;
                tool_state.circle.circle_y = c_y;
                tool_state.circle.circle_r_x = c_w;
                tool_state.circle.circle_r_y = c_h;
            }
            break;
        case SDL_MOUSEBUTTONUP:
            if(event->button.button == SDL_BUTTON_LEFT) {
                left_mouse_down = 0;
                /*if(event->button.x<left_mouse_down_x) {
                    tool_state.rect.rect_x = event->button.x;
                } else {
                    tool_state.rect.rect_x = left_mouse_down_x;
                }
                if(event->button.y<left_mouse_down_y) {
                    tool_state.rect.rect_y = event->button.y;
                } else {
                    tool_state.rect.rect_y = left_mouse_down_y;
                }
                tool_state.rect.rect_w = fabs(event->button.x - left_mouse_down_x);
                tool_state.rect.rect_h = fabs(event->button.y - left_mouse_down_y);*/

                // Do something
                int r_x, r_y, r_w, r_h;

                r_x = tool_state.rect.rect_x;
                r_y = tool_state.rect.rect_y;
                r_w = tool_state.rect.rect_w;
                r_h = tool_state.rect.rect_h;

                environment->coordsMouseToScreen(&tool_state.rect.rect_x,&tool_state.rect.rect_y,&tool_state.rect.rect_w,&tool_state.rect.rect_h);
                environment->coordsScreenToWorld(&tool_state.rect.rect_x,&tool_state.rect.rect_y,&tool_state.rect.rect_w,&tool_state.rect.rect_h);

                int c_x, c_y, c_w, c_h;

                c_x = tool_state.circle.circle_x;
                c_y = tool_state.circle.circle_y;
                c_w = tool_state.circle.circle_r_x;
                c_h = tool_state.circle.circle_r_y;

                environment->coordsMouseToScreen(&tool_state.circle.circle_x,&tool_state.circle.circle_y,&tool_state.circle.circle_r_x,&tool_state.circle.circle_r_y);
                environment->coordsScreenToWorld(&tool_state.circle.circle_x,&tool_state.circle.circle_y,&tool_state.circle.circle_r_x,&tool_state.circle.circle_r_y);

                if(tool_state.operation==LC_TOOL_OPERATION_FILL) {
                    if(environment->isMouseOver()) environment->act(&tool_state);
                }

                tool_state.rect.rect_x = r_x;
                tool_state.rect.rect_y = r_y;
                tool_state.rect.rect_w = r_w;
                tool_state.rect.rect_h = r_h;
                tool_state.circle.circle_x = c_x;
                tool_state.circle.circle_y = c_y;
                tool_state.circle.circle_r_x = c_w;
                tool_state.circle.circle_r_y = c_h;

                /*switch(tool_state.type) {
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
                                                                for(int i = tool_state.rect.rect_x; i < (tool_state.rect.rect_x+tool_state.rect.rect_w); i++) {
                                                                    for(int j = tool_state.rect.rect_y; j < (tool_state.rect.rect_y+tool_state.rect.rect_h); j++) {
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
                                                                for(int i = tool_state.rect.rect_x; i < (tool_state.rect.rect_x+tool_state.rect.rect_w); i++) {
                                                                    for(int j = tool_state.rect.rect_y; j < (tool_state.rect.rect_y+tool_state.rect.rect_h); j++) {
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
                                                                for(int i = tool_state.rect.rect_x; i < (tool_state.rect.rect_x+tool_state.rect.rect_x); i++) {
                                                                    for(int j = tool_state.rect.rect_y; j < (tool_state.rect.rect_y+tool_state.rect.rect_h); j++) {
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
                                                                for(int i = tool_state.rect.rect_x; i < (tool_state.rect.rect_x+tool_state.rect.rect_w); i++) {
                                                                    for(int j = tool_state.rect.rect_y; j < (tool_state.rect.rect_y+tool_state.rect.rect_h); j++) {
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
                }*/
                //environment->fillTerrain(tool_state.colour.r,tool_state.colour.g,tool_state.colour.b,this_w,this_h,this_x,this_y);

                tool_state.rect.rect_x = 0;
                tool_state.rect.rect_y = 0;
                tool_state.rect.rect_w = 0;
                tool_state.rect.rect_h = 0;
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

    if(menu_bg_color) {
        menu_bg_color->r = tool_state.colour.r;
        menu_bg_color->g = tool_state.colour.g;
        menu_bg_color->b = tool_state.colour.b;
        menu_bg_color->a = tool_state.colour.a;
    }

    LC_GamePanel::render();

    // Render tool
    if(tool_state.type!=LC_TOOL_TYPE_SELECT) {
        switch(tool_state.operation) {
            case LC_TOOL_OPERATION_FILL:
                switch(tool_state.shape) {
                    case LC_TOOL_SHAPE_RECT:
                        if(left_mouse_down) {
                            //cout << "x y w h = " << tool_state.rect.x << " " << tool_state.rect.y << " " << tool_state.rect.w << " " << tool_state.rect.h << " " << endl;
                            SDL_SetRenderDrawColor(renderer,255,0,0,255);
                            SDL_Rect this_rect;
                            this_rect.x = tool_state.rect.rect_x;
                            this_rect.y = tool_state.rect.rect_y;
                            this_rect.w = tool_state.rect.rect_w;
                            this_rect.h = tool_state.rect.rect_h;
                            SDL_RenderDrawRect(renderer,&this_rect);
                        }
                        break;
                    case LC_TOOL_SHAPE_CIRCLE:
                        if(left_mouse_down) {
                            SDL_SetRenderDrawColor(renderer,255,0,0,255);
                            tool_state.circle.line = 1;
                            tool_state.circle.env_w = screen_width;
                            tool_state.circle.env_h = screen_height;
                            for(int i = 0; i < tool_state.circle.getN(); i++) {
                                bool b;
                                int this_x, this_y;
                                b = tool_state.circle.iToXY(i,&this_x, &this_y);
                                if(b) {
                                    SDL_RenderDrawPoint(renderer,this_x,this_y);
                                }
                            }
                            tool_state.circle.line = 0;
                            tool_state.circle.env_w = environment->getWidth();
                            tool_state.circle.env_h = environment->getHeight();
                        }
                        break;
                }
                break;
            case LC_TOOL_OPERATION_DRAW:
            case LC_TOOL_OPERATION_SPRAY:
                switch(tool_state.shape) {
                    case LC_TOOL_SHAPE_CIRCLE:
                        {
                            SDL_SetRenderDrawColor(renderer,255,0,0,255);
                            tool_state.circle.line = 1;
                            tool_state.circle.env_w = screen_width;
                            tool_state.circle.env_h = screen_height;
                            for(int i = 0; i < tool_state.circle.getN(); i++) {
                                bool b;
                                int this_x, this_y;
                                b = tool_state.circle.iToXY(i,&this_x, &this_y);
                                if(b) {
                                    SDL_RenderDrawPoint(renderer,this_x,this_y);
                                }
                            }
                            tool_state.circle.line = 0;
                            tool_state.circle.env_w = environment->getWidth();
                            tool_state.circle.env_h = environment->getHeight();
                        }
                        break;
                    case LC_TOOL_SHAPE_RECT:
                        {
                            SDL_SetRenderDrawColor(renderer,255,0,0,255);
                            SDL_Rect this_rect;
                            this_rect.x = tool_state.rect.rect_x-tool_state.rect.rect_w/2;
                            this_rect.y = tool_state.rect.rect_y-tool_state.rect.rect_h/2;
                            this_rect.w = tool_state.rect.rect_w;
                            this_rect.h = tool_state.rect.rect_h;
                            SDL_RenderDrawRect(renderer,&this_rect);
                        }
                        break;
                }
                break;
        }
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
