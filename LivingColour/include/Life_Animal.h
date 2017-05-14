#ifndef LIFE_ANIMAL_INCLUDE
#define LIFE_ANIMAL_INCLUDE

#include "Life.h"
#include "Life_Action.h"

enum Gender {
    GENDER_NULL,
    GENDER_FEMALE,
    GENDER_MALE
};

struct Life_Animal : public Life {
    Gender gender = GENDER_NULL;
    Life_Action walk;
    Life_Action run;
    int move_direction_x = 0;
    int move_direction_y = 0;
    double probability_direction_change = 0.15;
};

#endif // LIFE_ANIMAL_INCLUDE
