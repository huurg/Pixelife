#ifndef LIFE_PLANT_HEADER
#define LIFE_PLANT_HEADER

#include "Life.h"
#include "Life_Action.h"

struct Life_Plant : public Life {
    Life_Action photosynthesize;
};

#endif // LIFE_PLANT_HEADER
