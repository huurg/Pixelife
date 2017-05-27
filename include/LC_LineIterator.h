#ifndef LC_LINE_ITERATOR_HEADER
#define LC_LINE_ITERATOR_HEADER

#include "LC_Iterator.h"

struct LC_LineIterator : public LC_Iterator {
    // Properties
    int x1 = 0;
    int x2 = 0;
    int y1 = 0;
    int y2 = 0;

    // Methods
    LC_LineIterator();
    bool iToXY(int i, int* ox, int* oy) const;
    int getN() const;
};

#endif // LC_LINE_ITERATOR_HEADER
