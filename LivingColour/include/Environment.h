#ifndef ENVIRONMENT_HEADER
#define ENVIRONMENT_HEADER

#include <SDL.h>

#include "EnvironmentObject.h"
#include "Life.h"
#include "Terrain.h"
#include "ColourID.h"
#include "Life_Animal.h"
#include "Life_Herbivore.h"
#include "Life_Plant.h"
#include "Life_Tree.h"

class Environment {
    public:
        int period = 1;
        int time = 0;
    private:
        int w;
        int h;
        int n_life = 0;
        int max_life = 100;
        int n_terrain = 0;
        int max_terrain = 100;
        Life*** life_grid = NULL;
        Life** life_list = NULL;
        Terrain*** terrain_grid = NULL;
        Terrain** terrain_list = NULL;

        SDL_Color** colour_map = NULL;

        SDL_Renderer* renderer = NULL;
        int screen_width;
        int screen_height;
        int screen_x_pos;
        int screen_y_pos;
        double camera_zoom = 1.0;
        int camera_x_pos = 0;
        int camera_y_pos = 0;
    public:
        Environment(int in_w, int in_h);
        ~Environment();

        void setRenderer(SDL_Renderer* in_renderer); // Must be set before rendering
        void setColourMapDefault();
        void render() const;
        void step();

        void updateHerbivore(Life_Herbivore* in_h);
        void updateLife(Life* in_l);

        bool moveLife(int life_ind, int dx, int dy);
        bool setLifePos(int life_ind, int in_x, int in_y);
        int spawnHerbivore(Gender in_gender = GENDER_MALE, int in_x = 0, int in_y = 0);
        int spawnHerbivores(double in_P);
        int clearTerrain(TerrainType inTT);
        void fillTerrain(TerrainType inTT, int in_w, int in_h, int in_x, int in_y);

        bool isOccupiable(int in_x, int in_y) const;
        LifeType lifeTypeAt(int in_x, int in_y) const;
        TerrainType terrainTypeAt(int in_x, int in_y) const;
        void coordsWorldToScreen(int* in_xp, int* in_yp) const;
        void coordsWorldToScreen(SDL_Rect* in_rect) const; //Note: Changes in_rect attributes
        void coordsScreenToWorld(SDL_Rect* in_rect) const;
        void coordsMouseToScreen(int* in_x, int* in_y) const;
        void moveCamera(int dx, int dy, double dz);
};

#endif // ENVIRONMENT_HEADER
