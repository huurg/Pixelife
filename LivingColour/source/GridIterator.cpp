#include "GridIterator.h"

#include <iostream>
using namespace std;

#include <utility>
#include <cmath>

GridIterator::GridIterator() {
    ;
}

GridIterator::GridIterator(int ix, int iy, int iw, int ih, int iD) {
    x = ix;
    y = iy;
    w = iw;
    h = ih;
    D = iD;
}

int GridIterator::idtor(int i, int d, int* ox, int* oy) const {
    int out = 1;
    if((i>=0)&&(i<(4*d))&&(d>=0)) {
        int ip = (int)((double)(i+2)/4.0);
        bool bx = ((int)((i+2)/2.0))%2;
        bool by = (i+2)%2;
        if(i>=(4*d-2)) std::swap(bx,by);
        int this_x = x + pow(-1.0,bx)*ip;
        int this_y = y + pow(-1.0,by)*(d-ip);
        if((this_x<0)||(this_x>=w)||(this_y<0)||(this_y>=h)) {
            out = 0;
        }
        while(this_x<0) this_x+=w;
        while(this_y<0) this_y+=h;
        this_x %= w;
        this_y %= h;
        *ox = this_x;
        *oy = this_y;
    } else if(d==0) {
        int this_x = x;
        int this_y = y;
        *ox = this_x;
        *oy = this_y;
    } else {
        out = -1;
        cout << "GridSelector::idtor error! Index and distance must be positive." << endl;
    }
    return out;
}

void GridIterator::print() const {
    for(int d = 0; d <= D; d++) {
        for(int i = 0; i < (4*d); i++) {
            int x_i, y_i, r_i;
            r_i = idtor(i, d, &x_i, &y_i);
            cout << "(d, i) = (" << d << ", " << i << ")\t(x, y, r) = (" << x_i << ", " << y_i << ", " << r_i << ")" << endl;
        }
    }
}

int GridIterator::N(int d) const {
    int out = (d?4*d:1);
    return out;
}
