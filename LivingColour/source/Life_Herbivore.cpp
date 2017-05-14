#include "Life_Herbivore.h"

Life_Herbivore::Life_Herbivore(Gender in_gender) {
    type = LIFE_TYPE_HERBIVORE;
    gender = in_gender;
    switch(gender) {
        case GENDER_FEMALE:
            colour_id = COLOUR_ID_HERBIVORE_FEMALE;
            break;
        case GENDER_MALE:
            colour_id = COLOUR_ID_HERBIVORE_MALE;
            break;
    }
    occupiable = 0;
}
