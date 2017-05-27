#ifndef LC_GAMEPANEL_HEADER
#define LC_GAMEPANEL_HEADER

#include "LC_Button.h"

#include <iostream>
using namespace std;

#include <SDL.h>

const int LC_GAMEPANEL_DEFAULT_MAX_GAMEPANELS = 100;
const int LC_GAMEPANEL_DEFAULT_MAX_BUTTONS = 100;

class LC_GamePanel : public LC_Button {
protected:
    SDL_Renderer* renderer = NULL;

    LC_Button** buttons = NULL;
    int n_buttons = 0;
    int max_buttons = LC_GAMEPANEL_DEFAULT_MAX_BUTTONS;

    LC_GamePanel** gamepanels = NULL;
    int n_gamepanels = 0;
    int max_gamepanels = LC_GAMEPANEL_DEFAULT_MAX_GAMEPANELS;

    LC_RenderObject** render_objects = NULL;
    int n_render_objects = 0;
    int max_render_objects = 0;

    SDL_Rect bg_rect;

public:
    // Properties
    /*
    int screen_width = 0;
    int screen_height = 0;
    int screen_x_pos = 0;
    int screen_y_pos = 0;
    double world_width = 1.0;
    double camera_x_pos = 0.0;
    double camera_y_pos = 0.0;
    double camera_zoom = 1.0;
    */
    SDL_Colour* bg_colour = NULL;
    SDL_Texture* bg_image = NULL;

    // Methods
    LC_GamePanel();
    LC_GamePanel(int in_max_buttons, int in_max_gamepanels);
    ~LC_GamePanel();

    void resize(int in_max_buttons, int in_max_gamepanels);

    void setRenderer(SDL_Renderer* in_renderer);

        // Coordinate transformations
    /*
    void coordsScreenToWorld(double* ox = NULL, double* oy = NULL, double* ow = NULL, double* oh = NULL) const;
    void coordsScreenToWorld(int* ox = NULL, int* oy = NULL, int* ow = NULL, int* oh = NULL) const;
    void coordsWorldToScreen(int* ox = NULL, int* oy = NULL, int* ow = NULL, int* oh = NULL) const;
    void coordsMouseToScreen(int* ox = NULL, int* oy = NULL, int* ow = NULL, int* oh = NULL) const;
    void coordsScreenToMouse(int* ox = NULL, int* oy = NULL, int* ow = NULL, int* oh = NULL) const;
    void moveCamera(int dx, int dy, double dz);
    */

        // Event handlers
    virtual void windowUpdate(SDL_Event* event);
    virtual void keyUpdate(SDL_Event* event);
    virtual bool mouseUpdate(SDL_Event* event);
    virtual void frameUpdate();

    void render();

    bool isMouseOver(SDL_Event* event) const;
    bool isMouseOver() const;
    bool isMouseOverButton(int i, SDL_Event* event) const;
    bool isMouseOverButton(int i) const;
    void setButtonAction(int i, void (*in_func)(void), LC_ClickableState type);

    void addGamePanel(LC_GamePanel* in_gamepanel);
    void addButton(LC_Button* in_button);

    int getN_GamePanels() const;
    int getN_Buttons() const;
};

#endif // LC_GAMEPANEL_HEADER
