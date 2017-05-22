#ifndef GRID_SELECTOR_HEADER
#define GRID_SELECTOR_HEADER

#include <SDL.h>

struct GridIterator {
    // Properties
    int x = 0;
    int y = 0;
    int w = 0;
    int h = 0;
    int D = 0;

    // Constructor
    GridIterator();
    GridIterator(int ix, int iy, int iw, int ih, int iD);

    // Methods
    int idtor(int i, int d, int* ox, int* oy) const; //Returns 1 if r = (ox,oy) is out of bounds defined by Dims = (w,h), 0 otherwise. Returns -1 if i or d are out of bounds.
    int rtoid(int ix, int iy, int* oi, int* od) const ;
    int N(int d) const;
    void print() const;
};

#endif // GRID_SELECTOR_HEADER
