#ifndef ENVIRONMENT_OBJECT_INCLUDE
#define ENVIRONMENT_OBJECT_INCLUDE

#include "ColourID.h"
#include <SDL.h>

struct EnvironmentObject {
    SDL_Colour colour;
    bool occupiable = 1;
    int x = 0;
    int y = 0;
    int id = -1;
    ColourID colour_id = COLOUR_ID_NULL;
};

#endif // ENVIRONMENT_OBJECT_INCLUDE
