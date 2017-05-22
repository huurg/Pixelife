#ifndef TERRAIN_HEADER
#define TERRAIN_HEADER

#include "EnvironmentObject.h"

enum TerrainType {
    TERRAIN_TYPE_NULL,
    TERRAIN_TYPE_EARTH,
    TERRAIN_TYPE_WATER,
    TERRAIN_TYPE_WALL,
    TERRAIN_TYPE_STONE
};

struct Terrain: public EnvironmentObject {
    TerrainType terrain_type = TERRAIN_TYPE_NULL;
};

#endif // TERRAIN_HEADER
