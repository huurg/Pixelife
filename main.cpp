#include <SDL.h>
#include <iostream>
using namespace std;

#include <cstdlib>
#include <ctime>
#include <cmath>

#include "main_header.h"
#include "GridSelector.h"
#include "LC_Utils.h"
#include "LC_GamePanel.h"
#include "LC_MouseState.h"

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 960;
const bool FULL_SCREEN = 0;

SDL_Window* gWindow = NULL;
SDL_Surface* gScreenSurface = NULL;
SDL_Renderer* gRenderer = NULL;

Environment* gEnvironment = NULL;

LC_GamePanel createMenu();

void menu_click_action();
void button_click_action();

int main(int argc, char* args[]) {
    srand(time(NULL));

    SDL_Init(SDL_INIT_VIDEO);

    gWindow = SDL_CreateWindow("Living Colour", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT,FULL_SCREEN*SDL_WINDOW_FULLSCREEN);
    gScreenSurface = SDL_GetWindowSurface(gWindow);
    gRenderer = SDL_CreateRenderer(gWindow,-1,SDL_RENDERER_ACCELERATED|SDL_RENDERER_PRESENTVSYNC);
    SDL_SetRenderDrawBlendMode(gRenderer,SDL_BLENDMODE_BLEND);

    Environment my_env(256,192);
    gEnvironment = &my_env;

    //SDL_Texture* atex = LC_Utils::LC_LoadTexture(gRenderer,"luke.bmp");

    SDL_Rect my_rect;
    my_rect.h = 0;
    my_rect.w = 0;
    my_rect.x = 0;
    my_rect.y = 0;
    bool render_my_rect = false;
    bool create_mode = 1;

    SDL_Rect viewport_rect;
    double menu_bar_height = 0.05;
    viewport_rect.h = SCREEN_HEIGHT*(1.0-menu_bar_height);
    viewport_rect.w = SCREEN_WIDTH;
    viewport_rect.x = 0;
    viewport_rect.y = SCREEN_HEIGHT*menu_bar_height;

    LC_GamePanel menu_bar = createMenu();

    SDL_RenderSetViewport(gRenderer,&viewport_rect);
    my_env.setRenderer(gRenderer);

    bool exit = false;
    while(!exit) {
        SDL_RenderSetViewport(gRenderer,&viewport_rect);
        SDL_Event this_event;
        menu_bar.mouseUpdate(NULL);
        while(SDL_PollEvent(&this_event)) {
            if(this_event.type == SDL_WINDOWEVENT) {
                if(this_event.window.event == SDL_WINDOWEVENT_CLOSE) {
                    cout << "Exiting." << endl;
                    exit = true;
                } else if(this_event.window.event == SDL_WINDOWEVENT_MOVED) {
                    cout << "Moved to (" << this_event.window.data1 << ", " << this_event.window.data2 << ")." << endl;
                }
            } else if(this_event.type == SDL_KEYDOWN) {
                switch(this_event.key.keysym.scancode) {
                    case SDL_SCANCODE_ESCAPE:
                        cout << "Exiting." << endl;
                        exit = true;
                        //SDL_SetWindowFullscreen(gWindow,0);
                        break;
                    case SDL_SCANCODE_RIGHT:
                        my_env.moveCamera(1,0,0);
                        break;
                    case SDL_SCANCODE_LEFT:
                        my_env.moveCamera(-1,0,0);
                        break;
                    case SDL_SCANCODE_UP:
                        my_env.moveCamera(0,-1,0);
                        break;
                    case SDL_SCANCODE_DOWN:
                        my_env.moveCamera(0,1,0);
                        break;
                    case SDL_SCANCODE_Z:
                        my_env.moveCamera(0,0,0.1);
                        break;
                    case SDL_SCANCODE_X:
                        my_env.moveCamera(0,0,-0.1);
                        break;
                    case SDL_SCANCODE_LEFTBRACKET:
                        my_env.period++;
                        break;
                    case SDL_SCANCODE_RIGHTBRACKET:
                        if(my_env.period>0) my_env.period--;
                        break;
                    case SDL_SCANCODE_F:
                        my_env.spawnLives(0.01,50,100,200,GENDER_NULL,50);
                        break;
                    case SDL_SCANCODE_BACKSPACE:
                        my_env.clearTerrain(TERRAIN_TYPE_NULL);
                        break;
                    default:

                    break;
                }
            } else if(this_event.type==SDL_MOUSEBUTTONDOWN) {
                LC_isLeftMouseDown = 1;
                int this_mouse_x = this_event.button.x;
                int this_mouse_y = this_event.button.y;
                //cout << "Click at (" << this_mouse_x << ", " << this_mouse_y << ")" << endl;
                my_env.coordsMouseToScreen(&this_mouse_x,&this_mouse_y);
                //cout << "Selection at (" << this_mouse_x << ", " << this_mouse_y << ")" << endl;
                my_rect.x = this_mouse_x;
                my_rect.y = this_mouse_y;
                my_env.coordsScreenToWorld(&this_mouse_x,&this_mouse_y);
                my_env.printLifeAt(this_mouse_x,this_mouse_y);

                render_my_rect = true;
                if(this_event.button.button==SDL_BUTTON_LEFT) {
                    create_mode = true;
                } else if(this_event.button.button==SDL_BUTTON_RIGHT) {
                    create_mode = false;
                }
                menu_bar.LC_GamePanel::mouseUpdate(&this_event);
            } else if(this_event.type==SDL_MOUSEBUTTONUP) {
                LC_isLeftMouseDown = 0;
                if(my_rect.w<0) {
                    my_rect.x += my_rect.w;
                    my_rect.w *= -1;
                }
                if(my_rect.h<0) {
                    my_rect.y += my_rect.h;
                    my_rect.h *= -1;
                }
                my_env.coordsScreenToWorld(&my_rect);
                if(this_event.button.button==SDL_BUTTON_LEFT) {
                    my_env.fillTerrain(TERRAIN_TYPE_EARTH,my_rect.w,my_rect.h,my_rect.x,my_rect.y);
                } else if(this_event.button.button==SDL_BUTTON_RIGHT) {
                    my_env.fillTerrain(TERRAIN_TYPE_NULL,my_rect.w,my_rect.h,my_rect.x,my_rect.y);
                }

                render_my_rect = false;
                my_rect.h = 0;
                my_rect.w = 0;
                my_rect.x = 0;
                my_rect.y = 0;
                menu_bar.LC_GamePanel::mouseUpdate(&this_event);
            } else if(this_event.type==SDL_MOUSEMOTION) {
                if(render_my_rect) {
                    int this_mouse_x = this_event.motion.x;
                    int this_mouse_y = this_event.motion.y;
                    my_env.coordsMouseToScreen(&this_mouse_x,&this_mouse_y);
                    my_rect.w = -my_rect.x+this_mouse_x;
                    my_rect.h = -my_rect.y+this_mouse_y;
                }
                menu_bar.LC_GamePanel::mouseUpdate(&this_event);
            }
        }

        my_env.step();

        // Render step
        SDL_SetRenderDrawColor(gRenderer,0,0,0,255);
        SDL_RenderClear(gRenderer);


        my_env.render();
        SDL_RenderSetViewport(gRenderer,&viewport_rect);
        menu_bar.frameUpdate();
        menu_bar.render();
        SDL_RenderSetViewport(gRenderer,&viewport_rect);

        if(render_my_rect) {

            if(create_mode) {
                //SDL_RenderCopy(gRenderer,atex,NULL,&my_rect);
                SDL_SetRenderDrawColor(gRenderer,0,255,0,128);
            } else {
                SDL_SetRenderDrawColor(gRenderer,255,0,0,128);
            }
            SDL_RenderDrawRect(gRenderer,&my_rect);
        }
        SDL_RenderPresent(gRenderer);

    }
    cout << "Final time = " << my_env.time << endl;

    //SDL_FreeSurface(my_image);
    //SDL_FreeSurface(my_image2);

    SDL_DestroyRenderer(gRenderer);
    gRenderer = NULL;
    SDL_DestroyWindow(gWindow);
    gWindow = NULL;
    gScreenSurface = NULL;
    SDL_Quit();

    return 0;
}

LC_GamePanel createMenu() {
    LC_GamePanel menu_bar;
    menu_bar.mouseDownAction = menu_click_action;
    menu_bar.world_width = 300;
    menu_bar.setRenderer(gRenderer);
    SDL_Color* menu_bg_colour = new SDL_Colour;
    menu_bg_colour->r = 128;
    menu_bg_colour->g = 160;
    menu_bg_colour->b = 224;
    menu_bg_colour->a = 255;
    menu_bar.bg_colour = menu_bg_colour;
    menu_bar.screen_x_pos = menu_bar.screen_y_pos = 0;
    menu_bar.screen_width = SCREEN_WIDTH;
    menu_bar.screen_height = SCREEN_HEIGHT*0.05;
    SDL_Texture* bg_image = LC_Utils::LC_LoadTexture(gRenderer,"menu.bmp");
    menu_bar.bg_image = bg_image;
    LC_Button* my_button = new LC_Button;
    my_button->x = 1;
    my_button->y = 1;
    my_button->w = 9;
    my_button->h = 9;
    my_button->bg_colour[LC_BUTTON_ACTION_TYPE_UP] = new SDL_Colour;
    my_button->bg_colour[LC_BUTTON_ACTION_TYPE_OVER] = new SDL_Colour;
    my_button->bg_colour[LC_BUTTON_ACTION_TYPE_DOWN] = new SDL_Colour;
    my_button->bg_colour[LC_BUTTON_ACTION_TYPE_UP]->r = 255;
    my_button->bg_colour[LC_BUTTON_ACTION_TYPE_UP]->g = 255;
    my_button->bg_colour[LC_BUTTON_ACTION_TYPE_UP]->b = 255;
    my_button->bg_colour[LC_BUTTON_ACTION_TYPE_UP]->a = 255;
    my_button->bg_colour[LC_BUTTON_ACTION_TYPE_OVER]->r = 0;
    my_button->bg_colour[LC_BUTTON_ACTION_TYPE_OVER]->g = 255;
    my_button->bg_colour[LC_BUTTON_ACTION_TYPE_OVER]->b = 255;
    my_button->bg_colour[LC_BUTTON_ACTION_TYPE_OVER]->a = 255;
    my_button->bg_colour[LC_BUTTON_ACTION_TYPE_DOWN]->r = 255;
    my_button->bg_colour[LC_BUTTON_ACTION_TYPE_DOWN]->g = 0;
    my_button->bg_colour[LC_BUTTON_ACTION_TYPE_DOWN]->b = 0;
    my_button->bg_colour[LC_BUTTON_ACTION_TYPE_DOWN]->a = 255;
    my_button->mouseDownAction = button_click_action;
    menu_bar.addButton(my_button);

    return menu_bar;
}

void menu_click_action() {
    cout << "Menu click action!" << endl;
}

void button_click_action() {
    cout << "Button click action!" << endl;
    gEnvironment->step(true);
}
