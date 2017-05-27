#ifndef LC_CIRCLE_ITERATOR_HEADER
#define LC_CIRCLE_ITERATOR_HEADER

#include "LC_Iterator.h"

struct LC_CircleIterator : public LC_Iterator {
    // Properties
    bool centred = 0;
    int circle_x = 0;
    int circle_y = 0;
    int circle_r_x = 0;
    int circle_r_y = 0;
    bool line = 0;
    int thickness = 2;

    // Methods
    LC_CircleIterator();
    bool iToXY(int i, int* ox, int* oy) const;
    int getN() const;
};

#endif // LC_CIRCLE_ITERATOR_HEADER
