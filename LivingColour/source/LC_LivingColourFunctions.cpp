#include "LC_LivingColour.h"

#include <iostream>
using namespace std;

#include <SDL.h>

#include "LC_GamePanel.h"
#include "LC_MenuBar.h"

LC_LivingColour* LC_living_colour = NULL;

void LC_StartGame() {

    LC_living_colour = new LC_LivingColour();

    LC_living_colour->init();
    LC_living_colour->mainLoop();
    LC_living_colour->deinit();

    delete LC_living_colour;
    LC_living_colour = NULL;

}

int LC_GetScreenWidth() {
    int out = 0;
    if(LC_living_colour) {
        out = LC_living_colour->screen_width;
    }
    return out;
}

int LC_GetScreenHeight() {
    int out = 0;
    if(LC_living_colour) {
        out = LC_living_colour->screen_height;
    }
    return out;
}

bool LC_isLeftMouseDown() {
    bool out = 0;
    if(LC_living_colour) {
        out = LC_living_colour->isLeftMouseDown();
    }
    return out;
}

int LC_getLeftMouseDownX() {
    int out = 0;
    if(LC_living_colour) {
        out = LC_living_colour->getLeftMouseDownX();
    }
    return out;
}

int LC_getLeftMouseDownY() {
    int out = 0;
    if(LC_living_colour) {
        out = LC_living_colour->getLeftMouseDownY();
    }
    return out;
}

void LC_Step() {
    if(LC_living_colour) {
        LC_living_colour->environment->step(true);
    }
}

void LC_Play() {
    if(LC_living_colour) {
        LC_living_colour->environment->paused = 0;
    }
}

void LC_Pause() {
    if(LC_living_colour) {
        LC_living_colour->environment->paused = 1;
    }
}

void LC_SelectTool1() {
    if(LC_living_colour) {
        LC_living_colour->tool_state.type = LC_TOOL_TYPE_CREATE;
        LC_living_colour->tool_state.target = LC_TOOL_TARGET_TERRAIN;
        LC_living_colour->tool_state.operation = LC_TOOL_OPERATION_FILL;
        LC_living_colour->tool_state.shape = LC_TOOL_SHAPE_RECT;
        LC_living_colour->tool_state.colour.r = LC_living_colour->tool_state.colour.g = LC_living_colour->tool_state.colour.b = LC_living_colour->tool_state.colour.a = 255;

    }
}

void LC_SelectTool2() {
    if(LC_living_colour) {
        LC_living_colour->tool_state.type = LC_TOOL_TYPE_ERASE;
        LC_living_colour->tool_state.target = LC_TOOL_TARGET_TERRAIN;
        LC_living_colour->tool_state.operation = LC_TOOL_OPERATION_FILL;
        LC_living_colour->tool_state.shape = LC_TOOL_SHAPE_RECT;

    }
}

void LC_SelectTool3() {
    if(LC_living_colour) {
        LC_living_colour->tool_state.type = LC_TOOL_TYPE_CREATE;
        LC_living_colour->tool_state.target = LC_TOOL_TARGET_LIFE;
        LC_living_colour->tool_state.operation = LC_TOOL_OPERATION_FILL;
        LC_living_colour->tool_state.shape = LC_TOOL_SHAPE_RECT;
        LC_living_colour->tool_state.colour.r = 0;
        LC_living_colour->tool_state.colour.g = 0;
        LC_living_colour->tool_state.colour.b = 255;
        LC_living_colour->tool_state.colour.a = 255;
    }
}

void LC_SelectTool4() {
    if(LC_living_colour) {
        LC_living_colour->tool_state.type = LC_TOOL_TYPE_ERASE;
        LC_living_colour->tool_state.target = LC_TOOL_TARGET_LIFE;
        LC_living_colour->tool_state.operation = LC_TOOL_OPERATION_FILL;
        LC_living_colour->tool_state.shape = LC_TOOL_SHAPE_RECT;
    }
}

void LC_IncrementPeriod() {
    if(LC_living_colour) {
        LC_living_colour->environment->period *= 2;
    }
}

void LC_DecrementPeriod() {
    if(LC_living_colour) {
        LC_living_colour->environment->period /= 2;
        if(LC_living_colour->environment->period<1) LC_living_colour->environment->period = 1;
    }
}
