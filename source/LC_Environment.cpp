#include "LC_Environment.h"

#include <iostream>
using namespace std;

#include <SDL.h>

#include "LC_GamePanel.h"

#include "LC_LivingColour.h"

#include "EnvironmentObject.h"
#include "Life.h"
#include "Terrain.h"
#include "ColourID.h"
#include "Life_Animal.h"
#include "Life_Herbivore.h"
#include "Life_Plant.h"
#include "Life_Tree.h"
#include "Utils.h"

LC_Environment::LC_Environment(int in_w, int in_h) : LC_GamePanel() {
    env_width = in_w;
    world_width = env_width;
    env_height = in_h;
    max_terrain = env_width*env_height;
    max_life = env_height*env_height;

    life_grid = new Life**[env_width];
    for(int x = 0; x < env_width; x++) {
        life_grid[x] = new Life*[env_height];
        for(int y = 0; y < env_height; y++) {
            life_grid[x][y] = NULL;
        }
    }
    life_list = new Life*[max_life];
    for(int i = 0; i < max_life; i++) life_list[i] = NULL;

    terrain_grid = new Terrain**[env_width];
    for(int x = 0; x < env_width; x++) {
        terrain_grid[x] = new Terrain*[env_height];
        for(int y = 0; y < env_height; y++) {
            terrain_grid[x][y] = NULL;
        }
    }
    terrain_list = new Terrain*[max_terrain];
    for(int i = 0; i < max_terrain; i++) terrain_list[i] = NULL;

    life_id_list = new int[max_life];
    terrain_id_list = new int[max_terrain];
}

LC_Environment::~LC_Environment() {
    //cout << "Are?" << endl;
    refreshIDList();
   for(int i = 0; i < env_width; i++) {
        for(int j = 0; j < env_height; j++) {
            eraseLife(i,j);
            spawnTerrain(-1,-1,-1,i,j);
        }
   }
   /*
   for(int i = 0; i < max_terrain; i++) {
        if(terrain_list[i]) {
            int temp_x = terrain_list[i]->x;
            int temp_y = terrain_list[i]->y;
            delete terrain_list[i];
            terrain_list[i] = NULL;
            terrain_grid[temp_x][temp_y] = NULL;
        }
    }*/

    delete [] terrain_list;
    terrain_list = NULL;
    for(int x = 0; x < env_width; x++) {
        delete [] terrain_grid[x];
        terrain_grid[x] = NULL;
    }
    delete [] terrain_grid;
    terrain_grid = NULL;
    /*
    for(int i = 0; i < max_life; i++) {
        if(life_list[i]) {
            int temp_x = life_list[i]->x;
            int temp_y = life_list[i]->y;
            delete life_list[i];
            life_list[i] = NULL;
            life_grid[temp_x][temp_y] = NULL;
        }
    }
    */
    delete [] life_list;
    life_list = NULL;
    for(int x = 0; x < env_width; x++) {
        delete [] life_grid[x];
        life_grid[x] = NULL;
    }
    delete [] life_grid;
    life_grid = NULL;

    delete [] life_id_list;
    life_id_list = NULL;
    delete [] terrain_id_list;
    terrain_id_list = NULL;
    //cout << "WHY?" << endl;
}

void LC_Environment::render() {
    LC_GamePanel::render();
    SDL_Rect this_rect;
    this_rect.x = 0;
    this_rect.y = 0;
    this_rect.w = 1;
    this_rect.h = 1;
    /*
    for(int x = 0; x < env_width; x++) {
        for(int y = 0; y < env_height; y++) {
            this_rect.x = x;
            this_rect.y = y;
            this_rect.w = 1;
            this_rect.h = 1;
            coordsWorldToScreen(&(this_rect.x),&(this_rect.y),&(this_rect.w),&(this_rect.h));

            if(life_grid[x][y]) {
                Life* this_life = life_grid[x][y];
                SDL_Color* this_colour = &this_life->colour;
                SDL_SetRenderDrawColor(renderer, this_colour->r, this_colour->g, this_colour->b, this_colour->a);
                SDL_RenderFillRect(renderer,&this_rect);
            } else if(terrain_grid[x][y]) {
                Terrain* this_terrain = terrain_grid[x][y];
                SDL_Color* this_colour = &this_terrain->colour;
                SDL_SetRenderDrawColor(renderer, this_colour->r, this_colour->g, this_colour->b, this_colour->a);
                SDL_RenderFillRect(renderer,&this_rect);
                //SDL_RenderDrawPoint(renderer,this_rect.x,this_rect.y);
                //cout << "x,y = " << this_rect.x << ", " << this_rect.x << endl;
                //cout << "w,h = " << this_rect.w << ", " << this_rect.h << endl;
            } else {
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                SDL_RenderFillRect(renderer,&this_rect);
            }
        }
    }
    */
    // Render terrain
    for(int i = 0; i < n_terrain_id; i++) {
        int this_ind = terrain_id_list[i];
        if((this_ind >= 0)&&(this_ind<max_terrain)) {
            Terrain* this_terrain = terrain_list[terrain_id_list[i]];
            if(this_terrain) {
                //cout << "ADFADF " << endl;
                this_rect.x = this_terrain->x;
                this_rect.y = this_terrain->y;
                this_rect.w = 1;
                this_rect.h = 1;
                coordsWorldToScreen(&(this_rect.x),&(this_rect.y),&(this_rect.w),&(this_rect.h));
                SDL_Color* this_colour = &this_terrain->colour;
                SDL_SetRenderDrawColor(renderer, this_colour->r, this_colour->g, this_colour->b, this_colour->a);
                SDL_RenderFillRect(renderer,&this_rect);
                //cout << "EEffdeef" << endl;
            }
        }
    }
    // Render life
    for(int i = 0; i < n_life_id; i++) {
        int this_ind = terrain_id_list[i];
        if((this_ind >= 0)&&(this_ind<max_terrain)) {
            Life* this_life = life_list[life_id_list[i]];
            if(this_life) {
                this_rect.x = this_life->x;
                this_rect.y = this_life->y;
                this_rect.w = 1;
                this_rect.h = 1;
                coordsWorldToScreen(&(this_rect.x),&(this_rect.y),&(this_rect.w),&(this_rect.h));
                SDL_Color* this_colour = &this_life->colour;
                SDL_SetRenderDrawColor(renderer, this_colour->r, this_colour->g, this_colour->b, this_colour->a);
                SDL_RenderFillRect(renderer,&this_rect);
            }
        }
    }
}

bool LC_Environment::isOccupiable(int in_x, int in_y) const {
    return (in_x>=0)&&(in_y>=0)&&(in_x<env_width)&&(in_y<env_height)&&(!life_grid[in_x][in_y])&&terrain_grid[in_x][in_y]&&terrain_grid[in_x][in_y]->occupiable;
}

int LC_Environment::nextLifeID() {
    int out = -1;
    if(n_life<max_life) {
        out = i_life;
        while(life_list[out]) {
            out++;
            out %= max_life;
        }
        i_life = out;
    }
    return out;
}

int LC_Environment::nextTerrainID() {
    int out = -1;
    if(n_terrain<max_terrain) {
        out = i_terrain;
        while(terrain_list[out]) {
            out++;
            out %= max_terrain;
        }
        i_terrain = out;
    }
    return out;
}

int LC_Environment::spawnLife(int in_r, int in_g, int in_b, Gender in_gender, int in_x, int in_y, int d_rgb) {
    int out = -1;
    if(isOccupiable(in_x,in_y)) {
        out = nextLifeID();
        if(out>=0) {
            if(in_r<0) in_r = rand()%256;
            if(in_g<0) in_g = rand()%256;
            if(in_b<0) in_b = rand()%256;
            Life* new_life = new Life(in_r,in_g,in_b,in_gender);
            if(d_rgb) new_life->varyColour(d_rgb);
            new_life->x = in_x;
            new_life->y = in_y;
            new_life->id = out;
            life_grid[in_x][in_y] = new_life;
            life_list[out] = new_life;
            n_life++;
        }
    }
    return out;
}

void LC_Environment::eraseLife(int ix, int iy) {
    if(ix<0) ix = 0;
    if(ix>(env_width-1)) ix = env_width-1;
    if(iy<0) iy = 0;
    if(iy>(env_height-1)) iy = env_height-1;

    if(life_grid[ix][iy]) {
        int temp_id = life_grid[ix][iy]->id;
        delete life_grid[ix][iy];
        life_grid[ix][iy] = NULL;
        life_list[temp_id] = NULL;
        n_life--;
    }
}

int LC_Environment::spawnTerrain(int in_r, int in_g, int in_b, int in_x, int in_y) {
    int out = -1;
    if(in_x<0) in_x = 0;
    if(in_x>(env_width-1)) in_x = env_width-1;
    if(in_y<0) in_y = 0;
    if(in_y>(env_height-1)) in_y = env_height-1;

    if(terrain_grid[in_x][in_y]) {
        int temp_id = terrain_grid[in_x][in_y]->id;
        delete terrain_grid[in_x][in_y];
        terrain_grid[in_x][in_y] = NULL;
        terrain_list[temp_id] = NULL;
        n_terrain--;
    }
    out = nextTerrainID();
    Terrain* new_terrain = NULL;
    if((in_r>=0)&&(in_g>=0)&&(in_b>=0)) {
        int this_id = nextTerrainID();
        if(this_id>=0) {
            new_terrain = new Terrain();
            new_terrain->x = in_x;
            new_terrain->y = in_y;
            new_terrain->id = this_id;
            new_terrain->colour.r = in_r;
            new_terrain->colour.g = in_g;
            new_terrain->colour.b = in_b;
            new_terrain->colour.a = 255;
            terrain_list[this_id] = new_terrain;
            n_terrain++;
        }
    }

    terrain_grid[in_x][in_y] = new_terrain;

    return out;
}

int LC_Environment::clearTerrain(int r, int g, int b) {
    for(int x = 0;  x < env_width; x++) {
        for(int y = 0; y < env_height; y++) {
            if(terrain_grid[x][y]) {
                int temp_id = terrain_grid[x][y]->id;
                delete terrain_grid[x][y];
                terrain_grid[x][y] = NULL;
                terrain_list[temp_id] = NULL;
                n_terrain--;
            }
            Terrain* new_terrain = NULL;

            if((r>=0)&&(g>=0)&&(b>=0)) {
                int this_id = nextTerrainID();
                if(this_id>=0) {
                    new_terrain = new Terrain();
                    new_terrain->x = x;
                    new_terrain->y = y;
                    new_terrain->id = this_id;
                    new_terrain->colour.r = r;
                    new_terrain->colour.g = g;
                    new_terrain->colour.b = b;
                    new_terrain->colour.a = 255;
                    terrain_list[this_id] = new_terrain;
                    n_terrain++;
                }
            }

            terrain_grid[x][y] = new_terrain;
        }
    }
}



void LC_Environment::fillTerrain(int r, int g, int b, int in_w, int in_h, int in_x, int in_y) {
    //cout << "max_terrain = " << max_terrain << endl;
    if(in_x<0) in_x = 0;
    if((in_x+in_w)>(env_width-1)) in_w = env_width-in_x-1;
    if(in_y<0) in_y = 0;
    if((in_y+in_h)>(env_height-1)) in_h = env_height-in_y-1;

    for(int x = in_x;  (x<(in_x+in_w))&&(x < env_width); x++) {
        for(int y = in_y; (y<(in_y+in_h))&&(y < env_height); y++) {
            if(terrain_grid[x][y]) {
                int temp_id = terrain_grid[x][y]->id;
                delete terrain_grid[x][y];
                terrain_grid[x][y] = NULL;
                terrain_list[temp_id] = NULL;
                n_terrain--;
            }
            Terrain* new_terrain = NULL;
            if((r>=0)&&(g>=0)&&(b>=0)) {
                int this_id = nextTerrainID();
                if(this_id>=0) {
                    new_terrain = new Terrain();
                    new_terrain->colour.r = r;
                    new_terrain->colour.g = g;
                    new_terrain->colour.b = b;
                    new_terrain->colour.a = 255;
                    new_terrain->x = x;
                    new_terrain->y = y;
                    new_terrain->id = this_id;
                    terrain_list[this_id] = new_terrain;
                    n_terrain++;
                }
                //cout << "new terrain at " << new_terrain->id << endl;
            }
            terrain_grid[x][y] = new_terrain;
        }
    }
}

int LC_Environment::spawnLives(double in_P,int in_r, int in_g, int in_b,Gender in_gender, int d_rgb) {
    int out = 0;
    for(int x = 0; x < env_width; x++) {
        for(int y = 0; y < env_height; y++) {
            if(Utils::rand_d()<in_P) {
                int this_ind = spawnLife(in_r,in_g,in_b,in_gender,x,y,d_rgb);
                if(this_ind!=-1) out++;
            }
        }
    }
    return out;
}

// TODO finish transferring Environment to LC_Environment

bool LC_Environment::moveLife(int life_ind, int dx, int dy) {
    bool out = 0;

    Life* this_life = life_list[life_ind];
    if(this_life) {
        int x = this_life->x;
        int y = this_life->y;
        int xp = x+dx;
        int yp = y+dy;
        while(xp<0) xp+=env_width;
        while(yp<0) yp+=env_height;
        xp%=env_width;
        yp%=env_height;
        if(isOccupiable(xp,yp)) {
            life_grid[xp][yp]=this_life;
            life_grid[x][y] = NULL;
            this_life->x = xp;
            this_life->y = yp;
            out = 1;
        }
    }

    return out;
}

void LC_Environment::step(bool once) {
    // Herbivores
    if(once||(!paused)&&((period>0)&&(!(time%period)))) {
        for(int i = 0; i < n_life_id; i++) {
            int this_id = life_id_list[i];
            Life* this_life = life_list[this_id];
            if(this_life) {
                updateLife(this_life);
            }
        }
    }
    time++;
}

void LC_Environment::updateLife(Life* in_l) {
    // Die
    if(in_l) {
        int this_x = in_l->x;
        int this_y = in_l->y;
        int this_id = in_l->id;
        if((!terrain_grid[this_x][this_y])||(!terrain_grid[this_x][this_y]->occupiable)) {
            eraseLife(this_x,this_y);
            //refreshIDList();
        }
    }
    if(in_l) {
        // Reproduce (initiated by male)
        // Eat/kill
        // Move
            // Try change direction
        if(in_l->can_move) {
            if(Utils::rand_d()<in_l->probability_direction_change) {
                // Either zero or one magnitude
                if(Utils::rand_d()<0.5) {
                    // Zero mag
                    in_l->move_direction_x = 0;
                    in_l->move_direction_y = 0;
                } else {
                    // One mag
                    // Either Vert or Horiz non-zero component
                    if(Utils::rand_d()<0.5) {
                        //Horiz
                        //Either positive or negative
                        if(Utils::rand_d()<0.5) {
                            in_l->move_direction_x = 1;
                            in_l->move_direction_y = 0;
                        } else {
                            in_l->move_direction_x = -1;
                            in_l->move_direction_y = 0;
                        }
                    } else {
                        //Vert
                        //Either positive or negative
                        if(Utils::rand_d()<0.5) {
                            in_l->move_direction_x = 0;
                            in_l->move_direction_y = 1;
                        } else {
                            in_l->move_direction_x = 0;
                            in_l->move_direction_y = -1;
                        }
                    }
                }
            }
            //Update position
            moveLife(in_l->id,in_l->move_direction_x,in_l->move_direction_y);
        }
    // Recover/idle
    // Decrement cooldowns/update do_state
    }
}

void LC_Environment::printLifeAt(int in_x, int in_y) const {
    if((in_x>=0)&&(in_y>=0)&&(in_x<env_width)&&(in_y<env_height)) {
        if(life_grid[in_x][in_y]) {
            life_grid[in_x][in_y]->print();
        } else {
            cout << "No life here." << endl;
        }
    }
}

void LC_Environment::printTerrainAt(int in_x, int in_y) const {
    if((in_x>=0)&&(in_y>=0)&&(in_x<env_width)&&(in_y<env_height)) {
        if(terrain_grid[in_x][in_y]) {
            terrain_grid[in_x][in_y]->print();
        } else {
            cout << "No terrain here." << endl;
        }
    }
}

int LC_Environment::getWidth() const {
    return env_width;
}

int LC_Environment::getHeight() const {
    return env_height;
}

void LC_Environment::frameUpdate() {
    LC_GamePanel::frameUpdate();
    step();
}

void LC_Environment::refreshIDList() {
    //cout << "ADFA" << endl;
    n_life_id = 0;
    n_terrain_id = 0;
    if(life_id_list) {
        for(int i = 0; i < max_life; i++) {
            if(life_list[i]) {
                life_id_list[n_life_id] = i;
                n_life_id++;
            }
        }
    }
    if(terrain_id_list) {
        for(int i = 0; i < max_terrain; i++) {
            if(terrain_list[i]) {
                terrain_id_list[n_terrain_id] = i;
                n_terrain_id++;
            }
        }
    }
    //cout << "Life count = " << n_life_id << endl;
    //cout << "Terrain count = " << n_terrain_id << endl;
}
