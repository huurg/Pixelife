#ifndef LC_DIAMOND_ITERATOR_HEADER
#define LC_DIAMOND_ITERATOR_HEADER

#include "LC_Iterator.h"

struct LC_DiamondIterator : public LC_Iterator {
    // Properties
    int diamond_d = 0;
    int diamond_x = 0;
    int diamond_y = 0;

    // Methods
    bool iToXY(int i, int* ox, int* oy) const;
    int getN() const;

    bool idToXY(int i, int d, int* ox, int* oy) const;
    int getN(int d) const;
};

#endif // LC_DIAMOND_ITERATOR_HEADER
