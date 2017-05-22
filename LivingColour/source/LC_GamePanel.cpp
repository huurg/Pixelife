#include "LC_GamePanel.h"

#include "LC_Button.h"
//#include "LC_MouseState.h"

#include <iostream>
using namespace std;

#include <SDL.h>

#include <cmath>
#include <cstdlib>
#include <utility>

#include "LC_LivingColour.h"

void LC_GamePanel::resize(int in_max_buttons, int in_max_gamepanels) {
    if(buttons) {
        LC_Button** temp_buttons = new LC_Button*[max_buttons];
        for(int i = 0; i < max_buttons; i++) {
            temp_buttons[i] = buttons[i];
        }
        delete [] buttons;
        int old_max_buttons = max_buttons;
        max_buttons = in_max_buttons;
        buttons = new LC_Button*[max_buttons];
        for(int i = 0; i < fmin(max_buttons,old_max_buttons); i++) {
            buttons[i] = temp_buttons[i];
        }
        delete [] temp_buttons;
    } else {
        max_buttons = in_max_buttons;
        buttons = new LC_Button*[max_buttons];
        for(int i = 0; i < max_buttons; i++) {
            buttons[i] = NULL;
        }
    }
    if(gamepanels) {
        LC_GamePanel** temp_gamepanels = new LC_GamePanel*[max_gamepanels];
        for(int i = 0; i < max_gamepanels; i++) {
            temp_gamepanels[i] = gamepanels[i];
        }
        delete [] gamepanels;
        int old_max_gamepanels = max_gamepanels;
        max_gamepanels = in_max_gamepanels;
        gamepanels = new LC_GamePanel*[max_gamepanels];
        for(int i = 0; i < fmin(max_gamepanels,old_max_gamepanels); i++) {
            gamepanels[i] = temp_gamepanels[i];
        }
        delete [] temp_gamepanels;
    } else {
        max_gamepanels = in_max_gamepanels;
        gamepanels = new LC_GamePanel*[max_gamepanels];
        for(int i = 0; i < max_gamepanels; i++) {
            gamepanels[i] = NULL;
        }
    }
}

LC_GamePanel::LC_GamePanel() {
    resize(max_buttons,max_gamepanels);
}

LC_GamePanel::LC_GamePanel(int in_max_buttons, int in_max_gamepanels) {
    resize(in_max_buttons,in_max_gamepanels);
}

LC_GamePanel::~LC_GamePanel() {
    if(buttons) {
        for(int i = 0; i < max_buttons; i++) {
            if(buttons[i]) delete buttons[i];
        }
        delete [] buttons;
    }
    if(gamepanels) {
        for(int i = 0; i < max_gamepanels; i++) {
            if(gamepanels[i]) delete gamepanels[i];
        }
        delete [] gamepanels;
    }
    if(LC_GamePanel::bg_colour) delete bg_colour;
    if(LC_GamePanel::bg_image) {
        SDL_DestroyTexture(LC_GamePanel::bg_image);
    }
}

void LC_GamePanel::setRenderer(SDL_Renderer* in_renderer) {
    if(in_renderer) {
        renderer = in_renderer;
    } else {
        cout << "Error! LC_GamePanel::setRenderer; Renderer is null." << endl;
    }
}

void LC_GamePanel::coordsScreenToWorld(double* ox, double* oy, double* ow, double* oh) const {
    if(ox) {
        double this_x = *ox;
        this_x = (world_width*(this_x)/(double)(camera_zoom*screen_width)+camera_x_pos);
        *ox = this_x;
    }
    if(oy) {
        double this_y = *oy;
        this_y = (world_width*(this_y)/(double)(camera_zoom*screen_width)+camera_y_pos);
        *oy = this_y;
    }
    if(ow) {
        double this_w = *ow;
        this_w = world_width*(this_w)/(double)(camera_zoom*screen_width);
        *ow = this_w;
    }
    if(oh) {
        double this_h = *oh;
        this_h = world_width*(this_h)/(double)(camera_zoom*screen_width);
        *oh = this_h;
    }
}

void LC_GamePanel::coordsWorldToScreen(int* ox, int* oy, int* ow, int* oh) const {
    if(ox) {
        int this_x = *ox;
        this_x = camera_zoom*screen_width*(this_x-camera_x_pos)/(double)world_width;
        *ox = this_x;
    }
    if(oy) {
        int this_y = *oy;
        this_y = camera_zoom*screen_width*(this_y-camera_y_pos)/(double)world_width;
        *oy = this_y;
    }
    if(ow) {
        int this_w = *ow;
        this_w = ceil(camera_zoom*screen_width*(this_w)/(double)world_width);
        *ow = this_w;
    }
    if(oh) {
        int this_h = *oh;
        this_h = ceil(camera_zoom*screen_width*(this_h)/(double)world_width);
        *oh = this_h;
    }
}

void LC_GamePanel::coordsMouseToScreen(int* ox, int* oy, int* ow, int* oh) const {
    if(ox) {
        int this_x = *ox;
        this_x = this_x - screen_x_pos;
        *ox = this_x;
    }
    if(oy) {
        int this_y = *oy;
        this_y = this_y - screen_y_pos;
        *oy = this_y;
    }
    if(ow) {
        int this_w = *ow;
        this_w = this_w;
        *ow = this_w;
    }
    if(oh) {
        int this_h = *oh;
        this_h = this_h;
        *oh = this_h;
    }
}

void LC_GamePanel::coordsScreenToMouse(int* ox, int* oy, int* ow, int* oh) const {
    if(ox) {
        int this_x = *ox;
        this_x = this_x + screen_x_pos;
        *ox = this_x;
    }
    if(oy) {
        int this_y = *oy;
        this_y = this_y + screen_y_pos;
        *oy = this_y;
    }
    if(ow) {
        int this_w = *ow;
        this_w = this_w;
        *ow = this_w;
    }
    if(oh) {
        int this_h = *oh;
        this_h = this_h;
        *oh = this_h;
    }
}

void LC_GamePanel::windowUpdate(SDL_Event* event) {
    for(int i = (n_gamepanels-1); i >= 0; i--) {
        if(gamepanels[i]) gamepanels[i]->windowUpdate(event);
    }
}

void LC_GamePanel::keyUpdate(SDL_Event* event) {
    for(int i = (n_gamepanels-1); i >= 0; i--) {
        if(gamepanels[i]) gamepanels[i]->keyUpdate(event);
    }
}

bool LC_GamePanel::mouseUpdate(SDL_Event* event) {
    bool anything_clicked = false;

    if(event) {
        switch(event->type) {
        case SDL_MOUSEMOTION:
            // If mouse is over button, then set button state to 'over' and trigger button action
            for(int i = (n_buttons-1); i >= 0; i--) {
                if(isMouseOverButton(i)) {
                    //cout << "Event: It's over!" << endl;
                    buttons[i]->state = LC_BUTTON_ACTION_TYPE_OVER;
                    if(buttons[i]->mouseOverAction) buttons[i]->mouseOverAction();
                } else if(buttons[i]->state != LC_BUTTON_ACTION_TYPE_UP) {
                    buttons[i]->state = LC_BUTTON_ACTION_TYPE_UP;
                }
            }
            break;
        case SDL_MOUSEBUTTONDOWN:
            // If button is in over state move to down state
            for(int i = (n_buttons-1); (!anything_clicked)&&(i >= 0); i--) {
                if(isMouseOverButton(i)) {
                    //cout << "Event: It's down!" << endl;
                    buttons[i]->state = LC_BUTTON_ACTION_TYPE_DOWN;
                    if(buttons[i]->mouseDownAction) buttons[i]->mouseDownAction();
                    anything_clicked = true;
                }
            }
            break;
        case SDL_MOUSEBUTTONUP:
            // If button is not in up state, put in up state and do up action
            for(int i = (n_buttons-1); i >= 0; i--) {
                if(buttons[i]->state != LC_BUTTON_ACTION_TYPE_UP) {
                    //cout << "Event: It's up!" << endl;
                    buttons[i]->state = LC_BUTTON_ACTION_TYPE_UP;
                    if(buttons[i]->mouseUpAction) buttons[i]->mouseUpAction();
                }
            }
            break;
        }


        for(int i = (n_gamepanels-1); (!anything_clicked)&&(i >= 0); i--) {
            if(gamepanels[i]) anything_clicked += gamepanels[i]->mouseUpdate(event);
        }
        if(!anything_clicked) {
            //cout << "mouse over = " << isMouseOver() << endl;
            switch(event->type) {
            case SDL_MOUSEMOTION:
                // If mouse is over this gamepane, then set button state to 'over' and trigger button action
                if(isMouseOver()) {
                    //cout << "OVER" << endl;
                    state = LC_BUTTON_ACTION_TYPE_OVER;
                    if(mouseOverAction) mouseOverAction();
                } else if(state != LC_BUTTON_ACTION_TYPE_UP) {
                    state = LC_BUTTON_ACTION_TYPE_UP;
                }
                break;
            case SDL_MOUSEBUTTONDOWN:
                // If this gamepane is in over state move to down state
                if(isMouseOver()) {
                    //cout << "DOWN" << endl;
                    state = LC_BUTTON_ACTION_TYPE_DOWN;
                    if(mouseDownAction) mouseDownAction();
                    anything_clicked = true;
                }
                break;
            case SDL_MOUSEBUTTONUP:
                // If this gamepane is not in up state, put in up state and do up action
                if(state != LC_BUTTON_ACTION_TYPE_UP) {
                    //cout << "UP" << endl;
                    state = LC_BUTTON_ACTION_TYPE_UP;
                    if(mouseUpAction) mouseUpAction();
                }
                break;
            }
        }
    }

    return anything_clicked;
}

void LC_GamePanel::frameUpdate() {
    for(int i = (n_buttons-1); i >= 0; i--) {
            //cout << "i = " << i << "; b = " << isMouseOverButton(i) << endl;
        if(isMouseOverButton(i)) {
            //cout << "o";
            buttons[i]->state = LC_BUTTON_ACTION_TYPE_OVER;
            //if(buttons[i]->mouseOverAction) buttons[i]->mouseOverAction();
            if(LC_isLeftMouseDown()) {
                //cout << "d";
                buttons[i]->state = LC_BUTTON_ACTION_TYPE_DOWN;
                //if(buttons[i]->mouseDownAction) buttons[i]->mouseDownAction();
                //anything_clicked = true;
            }
        } else if(buttons[i]->state != LC_BUTTON_ACTION_TYPE_UP) {
            //cout << "u";
            buttons[i]->state = LC_BUTTON_ACTION_TYPE_UP;
        }
    }

    if(isMouseOver()) {
        state = LC_BUTTON_ACTION_TYPE_OVER;
        if(LC_isLeftMouseDown()) {
            state = LC_BUTTON_ACTION_TYPE_DOWN;
        }
    } else {
        state = LC_BUTTON_ACTION_TYPE_UP;
    }
    for(int i = (n_gamepanels-1); i >= 0; i--) {
        if(gamepanels[i]) gamepanels[i]->frameUpdate();
    }
}

void LC_GamePanel::render() {
    if(renderer) {
        SDL_Rect viewport_rect;
        viewport_rect.x = screen_x_pos;
        viewport_rect.y = screen_y_pos;
        viewport_rect.w = screen_width;
        viewport_rect.h = screen_height;
        bg_rect.x = bg_rect.y = 0;
        bg_rect.w = screen_width;
        bg_rect.h = screen_height;

        SDL_RenderSetViewport(renderer,&viewport_rect);
        // Render background colour
        if(LC_GamePanel::bg_colour) {
            SDL_Color* this_colour = LC_GamePanel::bg_colour;
            SDL_SetRenderDrawColor(renderer,this_colour->r, this_colour->g, this_colour->b, this_colour->a);
            SDL_RenderFillRect(renderer,&bg_rect);
        }
        // Render background image
        if(LC_GamePanel::bg_image) {
            SDL_RenderCopy(renderer,bg_image,NULL,&bg_rect);
        }
        // Render other gamepanels
        for(int i = 0; i < n_gamepanels; i++) {
            gamepanels[i]->render();
        }
        // Render buttons
        for(int i = 0; i < n_buttons; i++) {
            SDL_Rect this_rect;
            this_rect.x = buttons[i]->x;
            this_rect.y = buttons[i]->y;
            this_rect.w = buttons[i]->w;
            this_rect.h = buttons[i]->h;
            coordsWorldToScreen(&this_rect.x,&this_rect.y,&this_rect.w,&this_rect.h);
            if(buttons[i]->bg_colour[buttons[i]->state]) {
                //cout << this_rect.x << " " << this_rect.y<< " "  << this_rect.w<< " "  << this_rect.h << endl;
                //cout << buttons[i]->state << endl;
                SDL_Color* this_colour = buttons[i]->bg_colour[buttons[i]->state];
                SDL_SetRenderDrawColor(renderer,this_colour->r, this_colour->g, this_colour->b, this_colour->a);
                SDL_RenderFillRect(renderer, &this_rect);
            }
            if(buttons[i]->bg_image[buttons[i]->state]) {
                SDL_RenderCopy(renderer,buttons[i]->bg_image[buttons[i]->state],NULL,&this_rect);
            }
        }
        SDL_RenderSetViewport(renderer,&viewport_rect);
    }
}

bool LC_GamePanel::isMouseOverButton(int i, SDL_Event* event) const {
    bool out = true;
    if(buttons[i]&&event) {
        if(event->type==SDL_MOUSEMOTION) {
            int mouse_x = event->motion.x;
            int mouse_y = event->motion.y;

            coordsMouseToScreen(&mouse_x,&mouse_y);
            double mouse_x_d = mouse_x;
            double mouse_y_d = mouse_y;
            coordsScreenToWorld(&mouse_x_d,&mouse_y_d);

            double left_x = buttons[i]->x;
            double right_x = buttons[i]->x + buttons[i]->w;
            if(left_x>right_x) swap(left_x, right_x);

            double top_y = buttons[i]->y;
            double bottom_y = buttons[i]->y + buttons[i]->h;
            if(top_y>bottom_y) swap(top_y,bottom_y);

            if((mouse_x_d<left_x)||(mouse_x_d>right_x)||(mouse_y_d<top_y)||(mouse_y_d>bottom_y)) out = false;

        } else {
            out = false;
        }
    } else {
        out = false;
    }
    return out;
}

bool LC_GamePanel::isMouseOverButton(int i) const {
    bool out = true;
    if(buttons[i]) {
            int mouse_x;
            int mouse_y;
            SDL_GetMouseState(&mouse_x,&mouse_y);

            coordsMouseToScreen(&mouse_x,&mouse_y);
            double mouse_x_d = mouse_x;
            double mouse_y_d = mouse_y;
            coordsScreenToWorld(&mouse_x_d,&mouse_y_d);

            double left_x = buttons[i]->x;
            double right_x = buttons[i]->x + buttons[i]->w;
            if(left_x>right_x) swap(left_x, right_x);

            double top_y = buttons[i]->y;
            double bottom_y = buttons[i]->y + buttons[i]->h;
            if(top_y>bottom_y) swap(top_y,bottom_y);

            if((mouse_x_d<left_x)||(mouse_x_d>right_x)||(mouse_y_d<top_y)||(mouse_y_d>bottom_y)) out = false;
            //cout << "mouse_r = (" << mouse_x_d << ", " << mouse_y_d << ")" << endl;
            //cout << "lr = (" << left_x << ", " << right_x << ")" << endl;
            //cout << "tb = (" << top_y << ", " << bottom_y << ")" << endl;
    } else {
        out = false;
    }
    return out;
}

bool LC_GamePanel::isMouseOver(SDL_Event* event) const {
    bool out = true;
    if(event) {
        if(event->type==SDL_MOUSEMOTION) {
            int mouse_x = event->motion.x;
            int mouse_y = event->motion.y;
            coordsMouseToScreen(&mouse_x,&mouse_y);

            int left_x = 0;
            int right_x = screen_width;
            if(left_x>right_x) swap(left_x, right_x);

            int top_y = y;
            int bottom_y = screen_height;
            if(top_y>bottom_y) swap(top_y,bottom_y);

            if((mouse_x<left_x)||(mouse_x>right_x)||(mouse_y<top_y)||(mouse_y>bottom_y)) out = false;
        } else {
            out = false;
        }
    } else {
        out = false;
    }
    return out;
}

bool LC_GamePanel::isMouseOver() const {
    bool out = true;
    int mouse_x;
    int mouse_y;
    SDL_GetMouseState(&mouse_x, &mouse_y);
    coordsMouseToScreen(&mouse_x,&mouse_y);



    int left_x = 0;
    int right_x = screen_width;
    if(left_x>right_x) swap(left_x, right_x);

    int top_y = y;
    int bottom_y = screen_height;
    if(top_y>bottom_y) swap(top_y,bottom_y);

    if((mouse_x<left_x)||(mouse_x>right_x)||(mouse_y<top_y)||(mouse_y>bottom_y)) out = false;

    return out;
}

void LC_GamePanel::addButton(LC_Button* in_button) {
    if(in_button&&(n_buttons<max_buttons)) {
        buttons[n_buttons] = in_button;
        n_buttons++;
    }
}

void LC_GamePanel::addGamePanel(LC_GamePanel* in_gamepanel) {
    if(in_gamepanel&&(n_gamepanels<max_gamepanels)) {
        gamepanels[n_gamepanels] = in_gamepanel;
        n_gamepanels++;
    }
}

void LC_GamePanel::moveCamera(int dx, int dy, double dz) {
    camera_x_pos += dx;
    camera_y_pos += dy;
    if((camera_zoom+dz)>(w/(double)screen_width))
    camera_zoom += dz;
}
