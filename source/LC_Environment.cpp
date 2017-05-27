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

    dia.env_w = env_width;
    dia.env_h = env_height;
    dia.env_periodic_boundary = 1;
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

int LC_Environment::spawnLife(int in_r, int in_g, int in_b, int in_x, int in_y) {
    int out = -1;
    if(isOccupiable(in_x,in_y)) {
        out = nextLifeID();
        if(out>=0) {
            Life* new_life = new Life(in_r,in_g,in_b,GENDER_NULL);
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
        if(life_grid[ix][iy]->child) {
            delete life_grid[ix][iy]->child;
            life_grid[ix][iy]->child = NULL;
        }
        if(life_grid[ix][iy]->parent) {
            delete life_grid[ix][iy]->parent;
            life_grid[ix][iy]->parent = NULL;
        }
        int temp_id = life_grid[ix][iy]->id;
        delete life_grid[ix][iy];
        life_grid[ix][iy] = NULL;
        life_list[temp_id] = NULL;
        n_life--;
    }
}

void LC_Environment::eraseTerrain(int ix, int iy) {
    if(ix<0) ix = 0;
    if(ix>(env_width-1)) ix = env_width-1;
    if(iy<0) iy = 0;
    if(iy>(env_height-1)) iy = env_height-1;

    if(terrain_grid[ix][iy]) {
        int temp_id = terrain_grid[ix][iy]->id;
        delete terrain_grid[ix][iy];
        terrain_grid[ix][iy] = NULL;
        terrain_list[temp_id] = NULL;
        n_terrain--;
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

void LC_Environment::act(LC_ToolState* tool_state) {
    LC_Iterator* this_iter = NULL;
    switch(tool_state->shape) {
        case LC_TOOL_SHAPE_RECT:
            this_iter = &tool_state->rect;
            break;
        case LC_TOOL_SHAPE_CIRCLE:
            this_iter = &tool_state->circle;
            break;
        case LC_TOOL_SHAPE_GRID:
            this_iter = &tool_state->diamond;
            break;
        case LC_TOOL_SHAPE_LINE:
            this_iter = &tool_state->line;
            break;
    }
    if(this_iter) {
        for(int i = 0; i < this_iter->getN(); i++) {
            bool b;
            int this_x, this_y;
            b = this_iter->iToXY(i,&this_x,&this_y);
            if(b&&((tool_state->operation!=LC_TOOL_OPERATION_SPRAY)||(Utils::rand_d()<tool_state->spray_density))) {
                switch(tool_state->type) {
                    case LC_TOOL_TYPE_CREATE:
                        switch(tool_state->target) {
                            case LC_TOOL_TARGET_LIFE:
                                spawnLife(tool_state->colour.r,tool_state->colour.g,tool_state->colour.b,this_x,this_y);
                                break;
                            case LC_TOOL_TARGET_TERRAIN:
                                //cout << "(x, y) = (" << this_x << ", " << this_y << ")" << endl;
                                //cout << "(r, g, b) = (" << (int)tool_state->colour.r << ", " << (int)tool_state->colour.g << ", " << (int)tool_state->colour.b << ")" << endl;
                                spawnTerrain(tool_state->colour.r,tool_state->colour.g,tool_state->colour.b,this_x,this_y);
                                break;
                            default:
                                break;
                        }
                        break;
                    case LC_TOOL_TYPE_ERASE:
                        switch(tool_state->target) {
                            case LC_TOOL_TARGET_LIFE:
                                eraseLife(this_x,this_y);
                                break;
                            case LC_TOOL_TARGET_TERRAIN:
                                eraseLife(this_x,this_y);
                                eraseTerrain(this_x,this_y);
                                break;
                            default:
                                break;
                        }
                        break;
                    default:
                        break;
                }
            }
        }
    }
    refreshIDList();
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
                /*if(i==0) {
                    cout << "energy = " << this_life->energy << endl;
                    cout << "stamina = " << this_life->stamina << endl;
                }*/
                updateLife(this_life);
            }
        }
    }
    //refreshIDList();
    time++;
}

void LC_Environment::updateLife(Life* in_l) {
    // Die
    if(in_l) {
        int this_x = in_l->x;
        int this_y = in_l->y;
        int this_id = in_l->id;
        if(in_l->can_die) {
            if((!terrain_grid[this_x][this_y])||(!terrain_grid[this_x][this_y]->occupiable)||(in_l->energy<=0.0)) {
                eraseLife(this_x,this_y);
                in_l = NULL;
                refreshIDList();
            }
        }
    }
    if(in_l) {
        if(in_l->alive) {
            if(in_l->can_eat_sun) {
                in_l->eat_efficiency = sun_efficiency;
            }

            Life_Action* this_la = in_l->getCurrentAction();
            if(in_l->can_do) {
                if(this_la->time<=0) {
                    bool chosen = 0;
                    if(this_la->action_end) {
                        // Do final frame actions
                        switch(in_l->do_state) {
                            case LIFE_DO_STATE_IDLE:
                                this_la->action_end = 0;
                                break;
                            case LIFE_DO_STATE_EAT:
                                this_la->action_end = 0;
                                break;
                            case LIFE_DO_STATE_DECAY:

                                break;
                            case LIFE_DO_STATE_WALK:
                                this_la->action_end = 0;
                                break;
                            case LIFE_DO_STATE_RUN:

                                break;
                            case LIFE_DO_STATE_MATE:
                                this_la->action_end = 0;
                                if((in_l->can_reproduce_sexually)&&(in_l->gender==GENDER_FEMALE)&&(in_l->child)) {
                                    //chosen = in_l->act(LIFE_DO_STATE_GESTATE);
                                    delete in_l->child;
                                    in_l->act(LIFE_DO_STATE_IDLE,true);
                                    //cout << "Mating failed!" << endl;
                                }
                                break;
                            case LIFE_DO_STATE_GESTATE:
                                // switch to give birth state
                                this_la->action_end = 0;
                                if((in_l->can_reproduce_sexually)&&(in_l->gender==GENDER_FEMALE)&&(in_l->child)) {
                                    chosen = in_l->act(LIFE_DO_STATE_GIVE_BIRTH);
                                    //if(chosen) cout << "Labour!" << endl;
                                }
                                break;
                            case LIFE_DO_STATE_GIVE_BIRTH:
                                this_la->action_end = 0;
                                if((in_l->can_reproduce_sexually)&&(in_l->gender==GENDER_FEMALE)&&(in_l->child)) {
                                    delete in_l->child;
                                    in_l->child = NULL;
                                }
                                // Try to give birth each frame until successful (perhaps add miscarriage feature
                                break;
                            case LIFE_DO_STATE_FLOWER:
                                this_la->action_end = 0;
                                break;
                            case LIFE_DO_STATE_GROW_SEED:
                                // switch to distribute seeds
                                this_la->action_end = 0;
                                chosen = in_l->act(LIFE_DO_STATE_DISTRIBUTE_SEED,true);
                                break;
                            case LIFE_DO_STATE_DISTRIBUTE_SEED:
                                this_la->action_end = 0;
                                if(in_l->parent) {
                                    delete in_l->parent;
                                    in_l->parent = NULL;
                                }
                                in_l->seeds = 0;
                                break;
                            case LIFE_DO_STATE_REST:
                                this_la->action_end = 0;
                                break;
                            default:
                                break;
                        }
                    }

                    // Choose new action:
                        // Try Reproduce
                    if(!chosen&&in_l->can_reproduce_sexually&&(Utils::rand_d()<in_l->probability_notice_partner)) {
                        Life* this_m = canMate(in_l->id);
                        if(this_m) {
                            chosen = in_l->act(LIFE_DO_STATE_MATE);
                            if(chosen) {
                                //cout << "Mated!" << endl;
                                in_l->move_direction_x = 0;
                                in_l->move_direction_y = 0;
                                this_m->move_direction_x = 0;
                                this_m->move_direction_y = 0;
                                this_m->act(LIFE_DO_STATE_MATE,true);
                                if(in_l->gender==GENDER_FEMALE) {
                                    in_l->pregnancy_probability = Life::mateProbability(in_l,this_m);
                                    in_l->child = Life::MixChromenes(in_l,this_m);
                                } else {
                                    in_l->pregnancy_probability = Life::mateProbability(in_l,this_m);
                                    this_m->child = Life::MixChromenes(in_l,this_m);
                                }
                            }

                        }
                    }
                    // Try pollenate
                    if(!chosen&&in_l->can_reproduce_asexually&&(Utils::rand_d()<in_l->probability_notice_partner)) {
                        chosen = in_l->act(LIFE_DO_STATE_FLOWER);
                        if(chosen) {
                            //cout << "Flowering!" << endl;
                        }
                    }

                        // Try Eat
                            // Try eat other life
                    if(!chosen&&!(Utils::rand_d()<in_l->probability_ignore_food)) {
                        Life* this_m = canEat(in_l->id);
                        if(this_m) {
                            chosen = in_l->act(LIFE_DO_STATE_EAT);
                            if(chosen) {
                                in_l->move_direction_x = 0.0;
                                in_l->move_direction_y = 0.0;
                                if(this_m->alive&&(!this_m->can_be_eaten_alive)) {
                                    this_m->alive = 0;
                                }
                                if(this_m->energy<(-in_l->eat.energy_cost)) {
                                    in_l->eat_cap = this_m->energy;
                                    this_m->energy -= in_l->eat_cap;
                                } else {
                                    in_l->eat_cap = -1.0;
                                    this_m->energy += in_l->eat.energy_cost;
                                }
                            }
                        }
                    }
                            // Try eat sun
                    if(!chosen&&!(Utils::rand_d()<in_l->probability_ignore_food)) {
                        if(in_l->can_eat_sun) {
                            chosen = in_l->act(LIFE_DO_STATE_EAT);
                        }
                    }
                        // Try flee/hunt
                        // Try move/idle
                    // Try resume labour
                    if((!chosen)&&(in_l->can_reproduce_sexually)&&(in_l->gender==GENDER_FEMALE)&&(in_l->child)) {
                        chosen = in_l->act(LIFE_DO_STATE_GESTATE,true);
                    }
                    if((!chosen)&&(in_l->can_move)&&(Utils::rand_d()<(1.0-in_l->probability_idle))) {
                        chosen = in_l->act(LIFE_DO_STATE_WALK);
                        if(chosen) {
                            in_l->randomizeDirection();
                            //Update position
                            moveLife(in_l->id,in_l->move_direction_x,in_l->move_direction_y);
                        }
                    }
                    if(!chosen) {
                        chosen = in_l->act(LIFE_DO_STATE_IDLE);
                        if(chosen) {
                            in_l->move_direction_x = 0;
                            in_l->move_direction_y = 0;
                        }
                    }
                } else {
                    // If flowering/mating check if fertilization successful:
                    // p' = 1 - (1 - mateProbability)^(1.0/duration)
                    // If successful switch immediately to gestate/grow seeds
                    // If distributing seeds, try to spread a seed every frame (with p') until all gone
                    switch(in_l->do_state) {
                        case LIFE_DO_STATE_GIVE_BIRTH:
                            {
                                dia.diamond_x = in_l->x;
                                dia.diamond_y = in_l->y;
                                dia.diamond_d = 1;
                                for(int i = 0; i < dia.getN(1); i++) {
                                    int this_x, this_y;
                                    bool b = dia.idToXY(i, 1, &this_x, &this_y);
                                    if(b&&(in_l->child)&&isOccupiable(this_x,this_y)) {
                                        int this_id = nextLifeID();
                                        life_list[this_id] = in_l->child;
                                        life_grid[this_x][this_y] = in_l->child;
                                        in_l->child->id = this_id;
                                        in_l->child->x = this_x;
                                        in_l->child->y = this_y;
                                        in_l->child += in_l->birth_energy;
                                        in_l->energy -= in_l->birth_energy;
                                        n_life++;
                                        refreshIDList();
                                        in_l->child = NULL;
                                        cout << "A birth!" << endl;
                                        LC_lives_born++;
                                        in_l->birth.time = 0;
                                        in_l->birth.action_end = 0;
                                    }
                                }
                            }
                            break;
                        case LIFE_DO_STATE_GESTATE:
                            //cout << "A";
                            break;
                        case LIFE_DO_STATE_MATE:
                            if(in_l->gender==GENDER_FEMALE) {
                                if(Utils::rand_d()<(1.0-pow(1.0-in_l->pregnancy_probability,1.0/in_l->mate.duration))) {
                                    //in_l->child = Life::MixChromenes(in_l,this_m);
                                    in_l->act(LIFE_DO_STATE_GESTATE,true);
                                    //cout << "Gestating!" << endl;
                                }
                            }
                            break;
                        case LIFE_DO_STATE_FLOWER:
                            if(in_l->can_reproduce_asexually) {
                                Life* this_m = canPollenate(in_l->id);
                                if(this_m) {
                                    //cout << "Mate prob = " << Life::mateProbability(in_l,this_m) << endl;
                                    //cout << "This frame = " << (1.0-pow(1.0-Life::mateProbability(in_l,this_m),1.0/in_l->flower.duration)) << endl;
                                    //cout << "Partner flowering = " << (this_m->do_state==LIFE_DO_STATE_FLOWER) << endl;
                                    //cout << "Partner:" << endl;
                                    //this_m->print();
                                    if((this_m->do_state==LIFE_DO_STATE_FLOWER)&&(Utils::rand_d()<(1.0-pow(1.0-Life::mateProbability(in_l,this_m),1.0/in_l->flower.duration)))) {

                                        if(!in_l->parent) in_l->parent = new Life((int)this_m->colour.r,(int)this_m->colour.g,(int)this_m->colour.b);
                                        in_l->act(LIFE_DO_STATE_GROW_SEED,true);
                                        //cout << "Making seeds: Parent = " << (bool)(in_l->parent) << endl;
                                    }
                                }
                            }
                            break;
                        case LIFE_DO_STATE_GROW_SEED:
                            //cout << "Trying to grow seeds..." << endl;
                            if(in_l->parent) {
                                //cout << "Parent:" << endl;
                                //in_l->parent->print();
                                double this_rand = Utils::rand_d();
                                //cout << "Probability = " << in_l->probability_grow << endl;
                                //cout << "Rand = " << this_rand << endl;
                                //cout << "This frame = " << (1.0-pow(1.0-in_l->probability_grow,1.0/in_l->grow_seed.duration)) << endl;

                                if(this_rand<(1.0-pow(1.0-in_l->probability_grow,1.0/in_l->grow_seed.duration))) {
                                    in_l->seeds++;
                                    //cout << "Seed grown!" << endl;
                                }
                            }
                            break;
                        case LIFE_DO_STATE_DISTRIBUTE_SEED:
                            if(in_l->parent&&(in_l->seeds>0)) {
                                if(Utils::rand_d()<(1.0-pow(1.0-in_l->probability_distribute,1.0/in_l->distribute_seed.duration))) {
                                    dia.diamond_x = in_l->x;
                                    dia.diamond_y = in_l->y;
                                    dia.diamond_d = in_l->seed_range;
                                    int this_i = Utils::rand_i(1,dia.getN()-1);
                                    int this_x, this_y;
                                    bool b = dia.iToXY(this_i,&this_x,&this_y);
                                    if(b&&isOccupiable(this_x,this_y)) {
                                        int this_id = nextLifeID();
                                        Life* this_child = Life::MixChromenes(in_l,in_l->parent);
                                        life_list[this_id] = this_child;
                                        life_grid[this_x][this_y] = this_child;
                                        this_child->id = this_id;
                                        this_child->x = this_x;
                                        this_child->y = this_y;
                                        this_child += in_l->seed_energy;
                                        in_l->energy -= in_l->seed_energy;
                                        n_life++;
                                        refreshIDList();
                                        cout << "Seed planted!" << endl;
                                        in_l->seeds--;
                                        LC_lives_born++;
                                    }
                                }
                            }
                    }
                }
            } else {
                // If no stamina, interrupt current action and force idle
                in_l->act(LIFE_DO_STATE_REST,true);
                in_l->move_direction_x = 0;
                in_l->move_direction_y = 0;
                in_l->can_do;
            }

            in_l->tick();

            /*if(in_l->can_move) {
                /*
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
                }*//*

            }*/
        // Recover/idle
        // Decrement cooldowns/update do_state
        } else {
            in_l->act(LIFE_DO_STATE_DECAY,true);
            in_l->tick();
        }
    }
}

void LC_Environment::printLifeAt(int in_x, int in_y) const {
    if((in_x>=0)&&(in_y>=0)&&(in_x<env_width)&&(in_y<env_height)) {
        if(terrain_grid[in_x][in_y]&&life_grid[in_x][in_y]) {
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

Life* LC_Environment::canEat(int i) {
    Life* out = NULL;

    Life* this_l = life_list[i];
    if(this_l&&this_l->alive) {
        int x = this_l->x;
        int y = this_l->y;
        dia.diamond_x = x;
        dia.diamond_y = y;
        dia.diamond_d = 1;
        for(int i = 0; (!out)&&(i < dia.getN(1)); i++) {
            int this_x, this_y;
            bool b = dia.idToXY(i,1,&this_x,&this_y);
            if(b) {
                Life* this_m = life_grid[this_x][this_y];
                if(this_m) {
                    if(Utils::rand_d()<Life::eatProbability(this_l,this_m)) {
                        out = this_m;
                    }
                }
            }
        }
    }

    return out;
}

Life* LC_Environment::canMate(int i) {
    Life* out = NULL;

    Life* this_l = life_list[i];
    if(this_l&&this_l->alive) {
        //cout << "1";
        int x = this_l->x;
        int y = this_l->y;
        dia.diamond_x = x;
        dia.diamond_y = y;
        dia.diamond_d = 1;
        for(int i = 0; (!out)&&(i < dia.getN(1)); i++) {
            int this_x, this_y;
            bool b = dia.idToXY(i,1,&this_x,&this_y);
            if(b) {
                //cout << "2";
                Life* this_m = life_grid[this_x][this_y];
                if(this_m) {
                    //cout << "3: " << Life::mateProbability(this_l,this_m);
                    if(this_m->do_state==LIFE_DO_STATE_IDLE) {
                        if(Utils::rand_d()<Life::mateProbability(this_l,this_m)) {
                            //cout << "4: " << Life::mateProbability(this_l,this_m);
                            out = this_m;
                        }
                    }
                }
            }
        }
    }
    //cout << endl;

    return out;
}

Life* LC_Environment::canPollenate(int i) {
    Life* out = NULL;

    Life* this_l = life_list[i];
    if(this_l&&this_l->alive) {
        //cout << "1";
        int x = this_l->x;
        int y = this_l->y;
        dia.diamond_x = x;
        dia.diamond_y = y;
        dia.diamond_d = this_l->flower_range;
        for(int d = 1; (!out)&&(d <= this_l->flower_range); d++) {
            for(int i = 0; (!out)&&(i < dia.getN(d)); i++) {
                int this_x, this_y;
                bool b = dia.idToXY(i,d,&this_x,&this_y);
                if(b) {
                    //cout << "2";
                    Life* this_m = life_grid[this_x][this_y];
                    if(this_m) {
                        //cout << "3: " << Life::mateProbability(this_l,this_m);
                        if(this_m->do_state==LIFE_DO_STATE_FLOWER) {
                            if(Utils::rand_d()<Life::mateProbability(this_l,this_m)) {
                                //cout << "4: " << Life::mateProbability(this_l,this_m);
                                out = this_m;
                            }
                        }
                    }
                }
            }
        }
    }
    //cout << endl;

    return out;
}
