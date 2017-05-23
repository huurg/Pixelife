#ifndef LC_ENVIRONMENT_HEADER
#define LC_ENVIRONMENT_HEADER

#include <iostream>
using namespace std;

#include <SDL.h>

#include "LC_GamePanel.h"
#include "EnvironmentObject.h"
#include "Life.h"
#include "Terrain.h"
#include "ColourID.h"
#include "Life_Animal.h"
#include "Life_Herbivore.h"
#include "Life_Plant.h"
#include "Life_Tree.h"

class LC_Environment : public LC_GamePanel {
    public:
        int period = 1;
        int time = 0;
        bool paused = 0;
    private:
        int env_width;
        int env_height;
        int i_life = 0;
        int n_life = 0;
        int max_life = 100;
        int i_terrain = 0;
        int n_terrain = 0;
        int max_terrain = 100;
        Life*** life_grid = NULL;
        Life** life_list = NULL;
        int* life_id_list = NULL;
        int n_life_id = 0;
        Terrain*** terrain_grid = NULL;
        Terrain** terrain_list = NULL;
        int* terrain_id_list = NULL;
        int n_terrain_id = 0;
    public:
        LC_Environment(int in_w, int in_h);
        ~LC_Environment();
        void updateLife(Life* in_l);

        bool moveLife(int life_ind, int dx, int dy);
        bool setLifePos(int life_ind, int in_x, int in_y);
        int spawnLife(int in_r = -1, int in_g = -1, int in_b = -1, Gender in_gender = GENDER_NULL, int in_x = 0, int in_y = 0, int d_rgb = 0); //Change to spawnLife
        int spawnTerrain(int in_r, int in_g, int in_b, int in_x, int in_y); //Change to spawnLife
        void eraseLife(int ix, int iy);

        int spawnLives(double in_P,int in_r = -1, int in_g = -1, int in_b = -1, Gender in_gender = GENDER_NULL, int d_rgb = 0); //Change to spawn lives
        void printLifeAt(int in_x, int in_y) const;
        void printTerrainAt(int in_x, int in_y) const;
        int clearTerrain(int r, int g, int b);
        void fillTerrain(int r, int g, int b, int in_w, int in_h, int in_x, int in_y);

        bool isOccupiable(int in_x, int in_y) const;
        LifeType lifeTypeAt(int in_x, int in_y) const;
        TerrainType terrainTypeAt(int in_x, int in_y) const;

        int nextLifeID();
        int nextTerrainID();

        void render();
        void step(bool once = false);

        int getWidth() const;
        int getHeight() const;

        void frameUpdate();

        void refreshIDList();
};

#endif // LC_ENVIRONMENT_HEADER
