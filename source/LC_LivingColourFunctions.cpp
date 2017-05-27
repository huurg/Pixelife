#include "LC_LivingColour.h"

#include <iostream>
using namespace std;

#include <SDL.h>

#include "LC_GamePanel.h"
#include "LC_MenuBar.h"

#include "Life.h"

#include <cstdlib>
#include <ctime>
#include <cmath>

LC_LivingColour* LC_living_colour = NULL;
int LC_lives_born = 0;

void LC_StartGame() {
    srand(time(NULL));
    /*Life A(0,30,255);
    Life B(25,0,254);
    A.gender = GENDER_FEMALE;
    B.gender = GENDER_MALE;
    A.print();
    B.print();
    Life* Cp = Life::MixChromenes(&A,&B);
    Cp->print();
    delete Cp;
    double result = Life::eatProbability(&A,&B);
    cout << "P(eat) = " << result << endl;
    result = Life::mateProbability(&A,&B);
    cout << "P(mate) = " << result << endl;*/

    LC_living_colour = new LC_LivingColour();

    LC_living_colour->init();
    LC_living_colour->mainLoop();
    LC_living_colour->deinit();

    cout << "Total lives born = " << LC_lives_born << endl;

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
        LC_living_colour->tool_state.operation = LC_TOOL_OPERATION_DRAW;
        LC_living_colour->tool_state.shape = LC_TOOL_SHAPE_CIRCLE;
        LC_living_colour->tool_state.circle.centred = 1;
        LC_living_colour->tool_state.circle.env_periodic_boundary = 0;
        LC_living_colour->tool_state.colour.r = LC_living_colour->tool_state.colour.g = LC_living_colour->tool_state.colour.b = LC_living_colour->tool_state.colour.a = 255;

    }
}

void LC_SelectTool2() {
    if(LC_living_colour) {
        LC_living_colour->tool_state.type = LC_TOOL_TYPE_ERASE;
        LC_living_colour->tool_state.target = LC_TOOL_TARGET_TERRAIN;
        LC_living_colour->tool_state.operation = LC_TOOL_OPERATION_DRAW;
        LC_living_colour->tool_state.shape = LC_TOOL_SHAPE_CIRCLE;
        LC_living_colour->tool_state.circle.centred = 1;
        LC_living_colour->tool_state.circle.env_periodic_boundary = 0;

    }
}

void LC_SelectTool3() {
    if(LC_living_colour) {
        LC_living_colour->tool_state.type = LC_TOOL_TYPE_CREATE;
        LC_living_colour->tool_state.target = LC_TOOL_TARGET_LIFE;
        LC_living_colour->tool_state.operation = LC_TOOL_OPERATION_DRAW;
        LC_living_colour->tool_state.shape = LC_TOOL_SHAPE_CIRCLE;
        LC_living_colour->tool_state.circle.centred = 1;
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
        LC_living_colour->tool_state.operation = LC_TOOL_OPERATION_DRAW;
        LC_living_colour->tool_state.shape = LC_TOOL_SHAPE_CIRCLE;
        LC_living_colour->tool_state.circle.centred = 1;
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

void LC_setToolTypeSelect() {
    if(LC_living_colour) {
        LC_living_colour->tool_state.type = LC_TOOL_TYPE_SELECT;
    }
}

void LC_setToolTypeCreate() {
    if(LC_living_colour) {
        LC_living_colour->tool_state.type = LC_TOOL_TYPE_CREATE;
    }
}

void LC_setToolTypeErase() {
    if(LC_living_colour) {
        LC_living_colour->tool_state.type = LC_TOOL_TYPE_ERASE;
    }
}

void LC_setToolTargetLife() {
    if(LC_living_colour) {
        LC_living_colour->tool_state.target = LC_TOOL_TARGET_LIFE;
    }
}

void LC_setToolTargetTerrain() {
    if(LC_living_colour) {
        LC_living_colour->tool_state.target = LC_TOOL_TARGET_TERRAIN;
    }
}

void LC_setToolOperationDraw() {
    if(LC_living_colour) {
        LC_living_colour->tool_state.operation = LC_TOOL_OPERATION_DRAW;
        LC_living_colour->tool_state.rect.centred = 1;
        LC_living_colour->tool_state.circle.centred = 1;
    }
}

void LC_setToolOperationFill() {
    if(LC_living_colour) {
        LC_living_colour->tool_state.operation = LC_TOOL_OPERATION_FILL;
        LC_living_colour->tool_state.rect.centred = 0;
        LC_living_colour->tool_state.circle.centred = 0;
    }
}

void LC_setToolOperationSpray() {
    if(LC_living_colour) {
        LC_living_colour->tool_state.operation = LC_TOOL_OPERATION_SPRAY;
        LC_living_colour->tool_state.rect.centred = 1;
        LC_living_colour->tool_state.circle.centred = 1;
    }
}

void LC_setToolShapeRect() {
    if(LC_living_colour) {
        LC_living_colour->tool_state.shape = LC_TOOL_SHAPE_RECT;
    }
}

void LC_setToolShapeCircle() {
    if(LC_living_colour) {
        LC_living_colour->tool_state.shape = LC_TOOL_SHAPE_CIRCLE;
    }
}

void LC_IncrementR() {
    if(LC_living_colour) {
        LC_living_colour->tool_state.colour.r++;
        if(LC_living_colour->tool_state.colour.r>255)
            LC_living_colour->tool_state.colour.r = 255;
    }
}

void LC_IncrementG() {
    if(LC_living_colour) {
        LC_living_colour->tool_state.colour.g++;
        if(LC_living_colour->tool_state.colour.g>255)
            LC_living_colour->tool_state.colour.g = 255;
    }
}

void LC_IncrementB() {
    if(LC_living_colour) {
        LC_living_colour->tool_state.colour.b++;
        if(LC_living_colour->tool_state.colour.b>255)
            LC_living_colour->tool_state.colour.b = 255;
    }
}

void LC_DecrementR() {
    if(LC_living_colour) {
        LC_living_colour->tool_state.colour.r--;
        if(LC_living_colour->tool_state.colour.r<0)
            LC_living_colour->tool_state.colour.r = 0;
    }
}

void LC_DecrementG() {
    if(LC_living_colour) {
        LC_living_colour->tool_state.colour.g--;
        if(LC_living_colour->tool_state.colour.g<0)
            LC_living_colour->tool_state.colour.g = 0;
    }
}

void LC_DecrementB() {
    if(LC_living_colour) {
        LC_living_colour->tool_state.colour.b--;
        if(LC_living_colour->tool_state.colour.b<0)
            LC_living_colour->tool_state.colour.b = 0;
    }
}

void LC_SetR() {
    if(LC_living_colour) {
        LC_living_colour->tool_state.colour.r = 255;
        LC_living_colour->tool_state.colour.g = 0;
        LC_living_colour->tool_state.colour.b = 0;
        LC_living_colour->tool_state.colour.a = 255;
    }
}

void LC_SetG() {
    if(LC_living_colour) {
        LC_living_colour->tool_state.colour.r = 0;
        LC_living_colour->tool_state.colour.g = 255;
        LC_living_colour->tool_state.colour.b = 0;
        LC_living_colour->tool_state.colour.a = 255;
    }
}

void LC_SetB() {
    if(LC_living_colour) {
        LC_living_colour->tool_state.colour.r = 0;
        LC_living_colour->tool_state.colour.g = 0;
        LC_living_colour->tool_state.colour.b = 255;
        LC_living_colour->tool_state.colour.a = 255;
    }
}

void LC_SetM() {
    if(LC_living_colour) {
        LC_living_colour->tool_state.colour.r = 255;
        LC_living_colour->tool_state.colour.g = 0;
        LC_living_colour->tool_state.colour.b = 255;
        LC_living_colour->tool_state.colour.a = 255;
    }
}

void LC_SetC() {
    if(LC_living_colour) {
        LC_living_colour->tool_state.colour.r = 0;
        LC_living_colour->tool_state.colour.g = 255;
        LC_living_colour->tool_state.colour.b = 255;
        LC_living_colour->tool_state.colour.a = 255;
    }
}

void LC_SetY() {
    if(LC_living_colour) {
        LC_living_colour->tool_state.colour.r = 255;
        LC_living_colour->tool_state.colour.g = 255;
        LC_living_colour->tool_state.colour.b = 0;
        LC_living_colour->tool_state.colour.a = 255;
    }
}

void LC_SetK() {
    if(LC_living_colour) {
        LC_living_colour->tool_state.colour.r = 0;
        LC_living_colour->tool_state.colour.g = 0;
        LC_living_colour->tool_state.colour.b = 0;
        LC_living_colour->tool_state.colour.a = 255;
    }
}

void LC_SetW() {
    if(LC_living_colour) {
        LC_living_colour->tool_state.colour.r = 255;
        LC_living_colour->tool_state.colour.g = 255;
        LC_living_colour->tool_state.colour.b = 255;
        LC_living_colour->tool_state.colour.a = 255;
    }
}

void LC_SetH() {
    if(LC_living_colour) {
        LC_living_colour->tool_state.colour.r = 128;
        LC_living_colour->tool_state.colour.g = 128;
        LC_living_colour->tool_state.colour.b = 128;
        LC_living_colour->tool_state.colour.a = 255;
    }
}

void LC_IncrementBrushSize() {
    if(LC_living_colour) {
        LC_living_colour->tool_state.brush_size++;
    }
}

void LC_DecrementBrushSize() {
    if(LC_living_colour) {
        LC_living_colour->tool_state.brush_size--;
        if(LC_living_colour->tool_state.brush_size<0) LC_living_colour->tool_state.brush_size = 0;
    }
}
