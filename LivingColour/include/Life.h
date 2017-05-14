#ifndef LIFE_INCLUDE
#define LIFE_INCLUDE

#include "EnvironmentObject.h"
#include "Life_Action.h"

enum LifeType {
    LIFE_TYPE_NULL,
    LIFE_TYPE_GRASS,
    LIFE_TYPE_TREE,
    LIFE_TYPE_HERBIVORE,
    LIFE_TYPE_PREDATOR
};

struct Life : public EnvironmentObject {
    int energy = 0;
    LifeType type = LIFE_TYPE_NULL;
    int stamina = 0;
    int stamina_max = 0;
    Life_Action recover_stamina;
    int speed = 0;
};

#endif // LIFE_INCLUDE
