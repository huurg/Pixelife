#include "LC_CircleIterator.h"

#include <iostream>
using namespace std;

LC_CircleIterator::LC_CircleIterator() {
    ;
}

int LC_CircleIterator::getN() const {
    int out = 0;

    out = 4*circle_r_x*circle_r_y;

    return out;
}

bool LC_CircleIterator::iToXY(int i, int* ox, int* oy) const {
    bool out = 1;

    int y = (i%(2*circle_r_y)) - circle_r_y;
    int x = (i/(2*circle_r_y)) - circle_r_x;

    if((circle_r_y*circle_r_y*x*x + circle_r_x*circle_r_x*y*y) > (circle_r_x*circle_r_y*circle_r_x*circle_r_y)) {
        out = 0;
    }
    if(line) {
        if(((circle_r_y-thickness)*(circle_r_y-thickness)*x*x + (circle_r_x-thickness)*(circle_r_x-thickness)*y*y) < ((circle_r_x-thickness)*(circle_r_y-thickness)*(circle_r_x-thickness)*(circle_r_y-thickness))) {
            out = 0;
        }
    }

    if(!centred) {
        x += circle_r_x;
        y += circle_r_y;
    }

    x += circle_x;
    y += circle_y;


    if((x<0)||(y<0)||(x>=env_w)||(y>=env_h)) {
        if(!env_periodic_boundary) {
            out = 0;
        }
    }
    while(x<0) x+=env_w;
    while(y<0) y+=env_h;
    x %= env_w;
    y %= env_h;

    *ox = x;
    *oy = y;

    return out;
}
