#include "LC_RectIterator.h"

#include <iostream>
using namespace std;

#include "LC_Iterator.h"

LC_RectIterator::LC_RectIterator() {
    ;
}

int LC_RectIterator::getN() const {
    int out = 0;

    out = rect_w*rect_h;

    return out;
}

bool LC_RectIterator::iToXY(int i, int* ox, int* oy) const {
    bool out = 1;
    // i = x*h + y
    // I = i*cols + j
    int y = (i%rect_h) + rect_y;
    int x = (i/rect_h) + rect_x;
    if(centred) {
        x -= (rect_w/2);
        y -= (rect_h/2);
    }

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
