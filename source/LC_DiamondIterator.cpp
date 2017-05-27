#include "LC_DiamondIterator.h"

#include <iostream>
using namespace std;

#include "LC_Iterator.h"

#include <cmath>

bool LC_DiamondIterator::idToXY(int i, int d, int* ox, int* oy) const {
    bool out = 1;

    if((i>=0)&&(i<(4*d))&&(d>=0)) {
        int ip = (int)((double)(i+2)/4.0);
        bool bx = ((int)((i+2)/2.0))%2;
        bool by = (i+2)%2;
        if(i>=(4*diamond_d-2)) std::swap(bx,by);
        int this_x = diamond_x + pow(-1.0,bx)*ip;
        int this_y = diamond_y + pow(-1.0,by)*(d-ip);
        if(!env_periodic_boundary) {
            if((this_x<0)||(this_x>=env_w)||(this_y<0)||(this_y>=env_h)) {
                out = 0;
            }
        }
        while(this_x<0) this_x+=env_w;
        while(this_y<0) this_y+=env_h;
        this_x %= env_w;
        this_y %= env_h;
        *ox = this_x;
        *oy = this_y;
    } else if(d==0) {
        int this_x = diamond_x;
        int this_y = diamond_y;
        *ox = this_x;
        *oy = this_y;
    } else {
        out = 0;
        cout << "GridSelector::idtor error! Index and distance must be positive." << endl;
    }

    return out;
}

int LC_DiamondIterator::getN() const {
    int out = 0;

    for(int d = 0; d <= diamond_d; d++) {
        out += getN(d);
    }

    return out;
}

bool LC_DiamondIterator::iToXY(int i, int* ox, int* oy) const {
    bool out = 1;

    int this_d = 0;
    int this_i = i;

    while(this_i>=getN(this_d)) {
        this_i -= getN(this_d);
        this_d++;
    }

    out = idToXY(this_i, this_d, ox, oy);

    return out;
}

int LC_DiamondIterator::getN(int d) const {
    int out = 0;

    if(d>0) {
        out = 4*d;
    } else {
        out = 1;
    }

    return out;
}
