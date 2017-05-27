#include "LC_ButtonGroup.h"

#include <SDL.h>

#include <iostream>
using namespace std;

LC_ButtonGroup::LC_ButtonGroup(int in_b, int in_gp) : LC_GamePanel(in_b,in_gp) {
    ;
}

bool LC_ButtonGroup::mouseUpdate(SDL_Event* event) {
    bool out = 0;

    out = LC_GamePanel::mouseUpdate(event);

    switch(event->type) {
        case SDL_MOUSEBUTTONDOWN:

            break;
        case SDL_MOUSEBUTTONUP:
            {
                for(int i = 0; i < n_buttons; i++) {
                    if(isMouseOverButton(i)) {
                        button_group_state = i;
                    }
                }
            }
            break;
        case SDL_MOUSEMOTION:

            break;
        default:
            break;
    }

    return out;
}

void LC_ButtonGroup::frameUpdate() {
    LC_GamePanel::frameUpdate();
    if(buttons[button_group_state]) {
        buttons[button_group_state]->click_state = LC_CLICKABLE_STATE_DOWN;
    }
}

void LC_ButtonGroup::render() {
    LC_GamePanel::render();
}

void LC_ButtonGroup::windowUpdate(SDL_Event* event) {
    LC_GamePanel::windowUpdate(event);
}

void LC_ButtonGroup::keyUpdate(SDL_Event* event) {
    LC_GamePanel::keyUpdate(event);
}

int LC_ButtonGroup::getButtonGroupState() const {
    return button_group_state;
}
