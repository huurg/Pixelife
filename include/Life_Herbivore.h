#ifndef LIFE_HERBIVORE_HEADER
#define LIFE_HERBIVORE_HEADER

#include "Life_Animal.h"

const int LIFE_HERBIVORE_DEFAULT_ENERGY = 100;
const int LIFE_HERBIVORE_DEFAULT_STAMINA_MAX = 100;

struct Life_Herbivore : public Life_Animal {
    Life_Herbivore(Gender in_gender);
};

#endif // LIFE_HERBIVORE_HEADER
