#ifndef LC_MENU_BAR_HEADER
#define LC_MENU_BAR_HEADER

#include <iostream>
using namespace std;

#include <SDL.h>

#include "LC_GamePanel.h"

constexpr char* LC_MENUBAR_STOPBUTTON_IMAGE = "stop.bmp";
constexpr char* LC_MENUBAR_PLAYBUTTON_IMAGE = "play.bmp";
constexpr char* LC_MENUBAR_STEPBUTTON_IMAGE = "step.bmp";

class LC_MenuBar : public LC_GamePanel {
    private:


    public:
        LC_MenuBar();
        ~LC_MenuBar();
        void init();

        void windowUpdate(SDL_Event* event);
        void keyUpdate(SDL_Event* event);
        bool mouseUpdate(SDL_Event* event);
        void frameUpdate();

        void render();
};

#endif // LC_MENU_BAR_HEADER
