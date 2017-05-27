#ifndef LC_LIVING_COLOUR_HEADER
#define LC_LIVING_COLOUR_HEADER

#include <iostream>
using namespace std;

#include <SDL.h>

#include "LC_GamePanel.h"
#include "LC_MenuBar.h"
#include "LC_Environment.h"
#include "LC_ToolState.h"

const int LC_INIT_SCREEN_WIDTH = 1920;
const int LC_INIT_SCREEN_HEIGHT = 1080;
const double LC_INIT_ENVIRONMENT_SCALE = 10.0;
const bool LC_INIT_FULL_SCREEN = 1;

class LC_LivingColour : public LC_GamePanel {
    private:
        SDL_Window* window = NULL;
        SDL_Surface* screen_surface = NULL;
        SDL_Renderer* renderer = NULL;
        bool full_screen = 0;
        bool left_mouse_down = 0;
        int left_mouse_down_x = 0;
        int left_mouse_down_y = 0;
        SDL_Color* menu_bg_color = NULL;
    public:
        // Properties
            // Tool state
        LC_ToolState tool_state;
            // Play state
            // Menu bar
        LC_MenuBar* menu_bar = NULL;
            // Environment
        LC_Environment* environment = NULL;
        bool exit = 0;

        // Methods
        LC_LivingColour();
        LC_LivingColour(int in_max_buttons, int in_max_gamepanels);
        ~LC_LivingColour();

        void init();
        void deinit();
        void mainLoop();

        void windowUpdate(SDL_Event* event);
        void keyUpdate(SDL_Event* event);
        bool mouseUpdate(SDL_Event* event);
        void frameUpdate();

        void render();

        // Display
        void toggleFullscreen();
        void setResolution(int in_screen_width, int in_screen_height);
        void resetDisplay();

        // Input
        bool isLeftMouseDown() const;
        int getLeftMouseDownX() const;
        int getLeftMouseDownY() const;
};

extern LC_LivingColour* LC_living_colour;
extern int LC_lives_born;

void LC_StartGame();
int LC_GetScreenWidth();
int LC_GetScreenHeight();
bool LC_isLeftMouseDown();
int LC_getLeftMouseDownX();
int LC_getLeftMouseDownY();
void LC_Step();
void LC_Play();
void LC_Pause();
void LC_SelectTool1();
void LC_SelectTool2();
void LC_SelectTool3();
void LC_SelectTool4();
void LC_IncrementPeriod();
void LC_DecrementPeriod();

void LC_setToolTypeSelect();
void LC_setToolTypeCreate();
void LC_setToolTypeErase();
void LC_setToolTargetLife();
void LC_setToolTargetTerrain();
void LC_setToolOperationDraw();
void LC_setToolOperationFill();
void LC_setToolOperationSpray();
void LC_setToolShapeRect();
void LC_setToolShapeCircle();

void LC_IncrementR();
void LC_IncrementG();
void LC_IncrementB();
void LC_DecrementR();
void LC_DecrementG();
void LC_DecrementB();
void LC_SetR();
void LC_SetG();
void LC_SetB();
void LC_SetM();
void LC_SetC();
void LC_SetY();
void LC_SetK();
void LC_SetW();
void LC_SetH();

void LC_IncrementBrushSize();
void LC_DecrementBrushSize();

#endif // LC_LIVING_COLOUR_HEADER
