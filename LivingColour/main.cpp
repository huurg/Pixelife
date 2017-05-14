#include <SDL.h>
#include <iostream>
using namespace std;

#include <cstdlib>
#include <ctime>
#include <cmath>

#include "main_header.h"

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 960;
const bool FULL_SCREEN = 0;

SDL_Window* gWindow = NULL;
SDL_Surface* gScreenSurface = NULL;
SDL_Renderer* gRenderer = NULL;

int main(int argc, char* args[]) {
    srand(time(NULL));

    SDL_Init(SDL_INIT_VIDEO);

    gWindow = SDL_CreateWindow("Living Colour", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT,FULL_SCREEN*SDL_WINDOW_FULLSCREEN);
    gScreenSurface = SDL_GetWindowSurface(gWindow);
    gRenderer = SDL_CreateRenderer(gWindow,-1,SDL_RENDERER_ACCELERATED|SDL_RENDERER_PRESENTVSYNC);
    SDL_SetRenderDrawBlendMode(gRenderer,SDL_BLENDMODE_BLEND);


    Environment my_env(256,192);

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

    SDL_RenderSetViewport(gRenderer,&viewport_rect);
    my_env.setRenderer(gRenderer);

    bool exit = false;
    while(!exit) {
        SDL_RenderSetViewport(gRenderer,&viewport_rect);
        SDL_Event this_event;
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
                        my_env.spawnHerbivores(0.01);
                        break;
                    case SDL_SCANCODE_BACKSPACE:
                        my_env.clearTerrain(TERRAIN_TYPE_NULL);
                        break;
                    default:

                    break;
                }
            } else if(this_event.type==SDL_MOUSEBUTTONDOWN) {
                int this_mouse_x = this_event.button.x;
                int this_mouse_y = this_event.button.y;
                cout << "Click at (" << this_mouse_x << ", " << this_mouse_y << ")" << endl;
                my_env.coordsMouseToScreen(&this_mouse_x,&this_mouse_y);
                cout << "Selection at (" << this_mouse_x << ", " << this_mouse_y << ")" << endl;
                my_rect.x = this_mouse_x;
                my_rect.y = this_mouse_y;

                render_my_rect = true;
                if(this_event.button.button==SDL_BUTTON_LEFT) {
                    create_mode = true;
                } else if(this_event.button.button==SDL_BUTTON_RIGHT) {
                    create_mode = false;
                }
            } else if(this_event.type==SDL_MOUSEBUTTONUP) {
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
            } else if(this_event.type==SDL_MOUSEMOTION) {
                if(render_my_rect) {
                    int this_mouse_x = this_event.motion.x;
                    int this_mouse_y = this_event.motion.y;
                    my_env.coordsMouseToScreen(&this_mouse_x,&this_mouse_y);
                    my_rect.w = -my_rect.x+this_mouse_x;
                    my_rect.h = -my_rect.y+this_mouse_y;
                }
            }
        }

        my_env.step();

        SDL_SetRenderDrawColor(gRenderer,0,0,0,255);
        SDL_RenderClear(gRenderer);

        my_env.render();
        if(render_my_rect) {
            if(create_mode) {
                SDL_SetRenderDrawColor(gRenderer,0,255,0,128);
            } else {
                SDL_SetRenderDrawColor(gRenderer,255,0,0,128);
            }
            SDL_RenderDrawRect(gRenderer,&my_rect);
        }
        SDL_RenderPresent(gRenderer);
    }
    cout << "Final time = " << my_env.time << endl;

    SDL_DestroyRenderer(gRenderer);
    gRenderer = NULL;
    SDL_DestroyWindow(gWindow);
    gWindow = NULL;
    gScreenSurface = NULL;
    SDL_Quit();

    return 0;
}
