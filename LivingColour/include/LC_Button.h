#ifndef LC_BUTTON_HEADER
#define LC_BUTTON_HEADER

#include <iostream>
using namespace std;

#include <SDL.h>

enum LC_ButtonActionType {
    LC_BUTTON_ACTION_TYPE_OVER,
    LC_BUTTON_ACTION_TYPE_DOWN,
    LC_BUTTON_ACTION_TYPE_UP,
    LC_BUTTON_ACTION_TYPE_N
};

struct LC_Button {
    // Properties
    // All units of length in world coordinate units
    double x = 0.0;
    double y = 0.0;
    double w = 0.0;
    double h = 0.0;
    LC_ButtonActionType state = LC_BUTTON_ACTION_TYPE_UP;
    void (*mouseOverAction)(void) = NULL;
    void (*mouseDownAction)(void) = NULL;
    void (*mouseUpAction)(void) = NULL;
    SDL_Colour** bg_colour = NULL;
    SDL_Texture** bg_image = NULL;

    // Methods
    LC_Button();

    void setGeometry(double ix, double iy, double iw, double ih);
    void setColour(int r, int g, int b, int a, LC_ButtonActionType t);
    void setColour(int r, int g, int b, int a);
    void setImage(SDL_Renderer* ir, const char* ifile, LC_ButtonActionType t,int a = 255);
    void setImage(SDL_Renderer* ir, const char* ifile,int a = 255);

    ~LC_Button();
};

#endif // LC_BUTTON_HEADER
