#ifndef LC_RECT_ITERATOR_HEADER
#define LC_RECT_ITERATOR_HEADER

#include "LC_Iterator.h"

struct LC_RectIterator : public LC_Iterator {
    // Properties
    bool centred = 1;
    int rect_x = 0;
    int rect_y = 0;
    int rect_w = 0;
    int rect_h = 0;

    // Methods
    LC_RectIterator();
    bool iToXY(int i, int* ox, int* oy) const;
    int getN() const;
};

#endif // LC_RECT_ITERATOR_HEADER
