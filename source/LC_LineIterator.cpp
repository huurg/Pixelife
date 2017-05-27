#include "LC_LineIterator.h"

#include <iostream>
using namespace std;

#include <cmath>

LC_LineIterator::LC_LineIterator() {
    ;
}

int LC_LineIterator::getN() const {
    int out = 0;

    int w = fabs(x2-x1);
    int h = fabs(y2-y1);

    out = w*h;

    if(!out) out = h;

    return out;
}

bool LC_LineIterator::iToXY(int i, int* ox, int* oy) const {
    bool out = 1;

    int w = fabs(x2-x1);
    int h = fabs(y2-y1);

    int x_min = fmin(x1,x2);
    int y_min = fmin(y1,y2);
    int x_max = fmax(x1,x2);
    int y_max = fmax(y1,y2);

    double y = (i%h);
    double x = (i/h);

    double alpha_0 = (double)(x*w+y*h)/(double)((w*w+h*h));
    double d = sqrt(x*x + y*y + alpha_0*alpha_0*(w*w+h*h) - 2.0*alpha_0*(x*w+y*h));
    //cout << "i = " << i << "; (x, y) = (" << x << ", " << y << "); alpha_0 = " << alpha_0 << "; d = " << d << endl;

    x = (int)x;
    y = (int)y;

    x += x_min;
    y += y_min;

    if((d>0.5)||(x<x_min)||(x>x_max)||(y<y_min)||(y>y_max)) {
        out = 0;
    }

    if((x<0)||(y<0)||(x>=env_w)||(y>=env_h)) {
        if(!env_periodic_boundary) {
            out = 0;
        }
    }
    while(x<0) x+=env_w;
    while(y<0) y+=env_h;
    x = (int)x%env_w;
    y = (int)y%env_h;

    *ox = x;
    *oy = y;

    return out;
}
