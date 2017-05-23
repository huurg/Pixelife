#include "Environment.h"

#include "EnvironmentObject.h"
#include "Life.h"
#include "Terrain.h"
#include "ColourID.h"
#include "Life_Action.h"
#include "Life_Animal.h"
#include "Life_Herbivore.h"
#include "Life_Plant.h"
#include "Terrain_Earth.h"
#include "Utils.h"
#include "GridIterator.h"

#include <iostream>
using namespace std;

#include <cmath>
#include <cstdlib>

Environment::Environment(int in_w, int in_h) {
    w = in_w;
    h = in_h;
    max_terrain = w*h;
    max_life = w*h;

    life_grid = new Life**[w];
    for(int x = 0; x < w; x++) {
        life_grid[x] = new Life*[h];
        for(int y = 0; y < h; y++) {
            life_grid[x][y] = NULL;
        }
    }
    life_list = new Life*[max_life];
    for(int i = 0; i < max_life; i++) life_list[i] = NULL;

    terrain_grid = new Terrain**[w];
    for(int x = 0; x < w; x++) {
        terrain_grid[x] = new Terrain*[h];
        for(int y = 0; y < h; y++) {
            terrain_grid[x][y] = NULL;
        }
    }
    terrain_list = new Terrain*[max_terrain];
    for(int i = 0; i < max_terrain; i++) terrain_list[i] = NULL;

    colour_map = new SDL_Color*[COLOUR_ID_N];
    for(int i = 0; i < COLOUR_ID_N; i++) colour_map[i] = new SDL_Color();

    setColourMapDefault();
}

Environment::~Environment() {
    for(int i = 0; i < COLOUR_ID_N; i++) delete colour_map[i];
    delete [] colour_map;

    for(int i = 0; i < max_terrain; i++) {
        if(terrain_list[i]) {
            int temp_x = terrain_list[i]->x;
            int temp_y = terrain_list[i]->y;
            delete terrain_list[i];
            terrain_list[i] = NULL;
            terrain_grid[temp_x][temp_y] = NULL;
        }
    }
    delete [] terrain_list;
    for(int x = 0; x < w; x++) {
        delete [] terrain_grid[x];
    }
    delete [] terrain_grid;

    for(int i = 0; i < max_life; i++) {
        if(life_list[i]) {
            int temp_x = life_list[i]->x;
            int temp_y = life_list[i]->y;
            delete life_list[i];
            life_list[i] = NULL;
            life_grid[temp_x][temp_y] = NULL;
        }
    }
    delete [] life_list;
    for(int x = 0; x < w; x++) {
        delete [] life_grid[x];
    }
    delete [] life_grid;
}

void Environment::setRenderer(SDL_Renderer* in_renderer) {
    renderer = in_renderer;
    SDL_Rect this_viewport;
    SDL_RenderGetViewport(renderer,&this_viewport);
    screen_width = this_viewport.w;
    screen_height = this_viewport.h;
    screen_x_pos = this_viewport.x;
    screen_y_pos = this_viewport.y;
    //SDL_GetRendererOutputSize(renderer,&screen_width,&screen_height);
    cout << "Environment output set to " << screen_width << " x " << screen_height << endl;
    cout << "Output position set to (" << screen_x_pos << ", " << screen_y_pos << ")" << endl;
}

void Environment::setColourMapDefault() {
    colour_map[COLOUR_ID_NULL]->r = 31;//34;
    colour_map[COLOUR_ID_NULL]->g = 31;//25;
    colour_map[COLOUR_ID_NULL]->b = 31;//13;
    colour_map[COLOUR_ID_NULL]->a = 255;

    colour_map[COLOUR_ID_EARTH_DAY]->r = 224;//161;
    colour_map[COLOUR_ID_EARTH_DAY]->g = 224;//125;
    colour_map[COLOUR_ID_EARTH_DAY]->b = 224;//87;
    colour_map[COLOUR_ID_EARTH_DAY]->a = 255;

    colour_map[COLOUR_ID_EARTH_NIGHT]->r = 5;
    colour_map[COLOUR_ID_EARTH_NIGHT]->g = 5;
    colour_map[COLOUR_ID_EARTH_NIGHT]->b = 20;
    colour_map[COLOUR_ID_EARTH_NIGHT]->a = 255;

    colour_map[COLOUR_ID_TREE]->r = 5;
    colour_map[COLOUR_ID_TREE]->g = 150;
    colour_map[COLOUR_ID_TREE]->b = 5;
    colour_map[COLOUR_ID_TREE]->a = 255;

    colour_map[COLOUR_ID_HERBIVORE_FEMALE]->r = 64;
    colour_map[COLOUR_ID_HERBIVORE_FEMALE]->g = 128;
    colour_map[COLOUR_ID_HERBIVORE_FEMALE]->b = 255;
    colour_map[COLOUR_ID_HERBIVORE_FEMALE]->a = 255;

    colour_map[COLOUR_ID_HERBIVORE_MALE]->r = 0;
    colour_map[COLOUR_ID_HERBIVORE_MALE]->g = 0;
    colour_map[COLOUR_ID_HERBIVORE_MALE]->b = 255;
    colour_map[COLOUR_ID_HERBIVORE_MALE]->a = 255;

}

void Environment::coordsWorldToScreen(int* in_xp, int* in_yp) const {
    int x_w = *in_xp;
    int y_w = *in_yp;

    int x_s = (int)(/*screen_x_pos+*/camera_zoom*screen_width*(x_w-camera_x_pos)/(double)w);
    int y_s = (int)(/*screen_y_pos+*/camera_zoom*screen_width*(y_w-camera_y_pos)/(double)w);

    *in_xp = x_s;
    *in_yp = y_s;
}

void Environment::coordsWorldToScreen(SDL_Rect* in_rect) const {
    int this_x = in_rect->x;
    int this_y = in_rect->y;
    coordsWorldToScreen(&this_x,&this_y);
    in_rect->x = this_x;
    in_rect->y = this_y;
    in_rect->w = (int)ceil(camera_zoom*screen_width*(in_rect->w)/(double)w);
    in_rect->h = (int)ceil(camera_zoom*screen_width*(in_rect->h)/(double)w);
}

void Environment::coordsScreenToWorld(SDL_Rect* in_rect) const {
    int this_x = in_rect->x;
    int this_y = in_rect->y;
    this_x = (int)(w*(this_x/*-screen_x_pos*/)/(double)(camera_zoom*screen_width)+camera_x_pos);
    this_y = (int)(w*(this_y/*-screen_y_pos*/)/(double)(camera_zoom*screen_width)+camera_y_pos);
    in_rect->x = this_x;
    in_rect->y = this_y;
    in_rect->w = (int)ceil(w*(in_rect->w)/(double)(camera_zoom*screen_width));
    in_rect->h = (int)ceil(w*(in_rect->h)/(double)(camera_zoom*screen_width));
}

void Environment::coordsScreenToWorld(int* in_xp, int* in_yp) const {
    int this_x = *in_xp;
    int this_y = *in_yp;
    this_x = (int)(w*(this_x/*-screen_x_pos*/)/(double)(camera_zoom*screen_width)+camera_x_pos);
    this_y = (int)(w*(this_y/*-screen_y_pos*/)/(double)(camera_zoom*screen_width)+camera_y_pos);
    (*in_xp) = this_x;
    (*in_yp) = this_y;
}

void Environment::coordsMouseToScreen(int* in_x, int* in_y) const {
    int this_x = (*in_x) - screen_x_pos;
    int this_y = (*in_y) - screen_y_pos;

    (*in_x) = this_x;
    (*in_y) = this_y;
}

void Environment::render() const {
    SDL_Rect this_rect;
    this_rect.x = 0;
    this_rect.y = 0;
    this_rect.w = 1;
    this_rect.h = 1;
    for(int x = 0; x < w; x++) {
        for(int y = 0; y < h; y++) {
            this_rect.x = x;
            this_rect.y = y;
            this_rect.w = 1;
            this_rect.h = 1;
            coordsWorldToScreen(&this_rect);
            if(life_grid[x][y]) {
                Life* this_life = life_grid[x][y];
                SDL_Color* this_colour = &this_life->colour;
                SDL_SetRenderDrawColor(renderer, this_colour->r, this_colour->g, this_colour->b, this_colour->a);
                SDL_RenderFillRect(renderer,&this_rect);
            } else if(terrain_grid[x][y]) {
                Terrain* this_terrain = terrain_grid[x][y];
                SDL_Color* this_colour = colour_map[this_terrain->colour_id];
                SDL_SetRenderDrawColor(renderer, this_colour->r, this_colour->g, this_colour->b, this_colour->a);
                SDL_RenderFillRect(renderer,&this_rect);
            } else {
                SDL_Color* this_colour = colour_map[COLOUR_ID_NULL];
                SDL_SetRenderDrawColor(renderer, this_colour->r, this_colour->g, this_colour->b, this_colour->a);
                SDL_RenderFillRect(renderer,&this_rect);
            }
        }
    }
}

void Environment::moveCamera(int dx, int dy, double dz) {
    camera_x_pos += dx;
    camera_y_pos += dy;
    if((camera_zoom+dz)>(w/(double)screen_width))
    camera_zoom += dz;
}

bool Environment::isOccupiable(int in_x, int in_y) const {
    return (!life_grid[in_x][in_y])&&terrain_grid[in_x][in_y]&&terrain_grid[in_x][in_y]->occupiable;
}

int Environment::spawnLife(int in_r, int in_g, int in_b, Gender in_gender, int in_x, int in_y, int d_rgb) {
    int out = -1;
    if(isOccupiable(in_x,in_y)) {
        bool found = false;
        for(int i = 0; (!found)&&(i<max_life); i++) {
            if(!(life_list[i])) {
                found = true;
                out = i;
                if(in_r<0) in_r = rand()%256;
                if(in_g<0) in_g = rand()%256;
                if(in_b<0) in_b = rand()%256;
                Life* new_life = new Life(in_r,in_g,in_b,in_gender);
                if(d_rgb) new_life->varyColour(d_rgb);
                new_life->x = in_x;
                new_life->y = in_y;
                new_life->id = i;
                life_grid[in_x][in_y] = new_life;
                life_list[i] = new_life;
            }
        }
    }
    return out;
}

int Environment::clearTerrain(TerrainType inTT) {
    int this_id = 0;
    for(int x = 0;  x < w; x++) {
        for(int y = 0; y < h; y++) {
            if(terrain_grid[x][y]) {
                int temp_id = terrain_grid[x][y]->id;
                delete terrain_grid[x][y];
                terrain_grid[x][y] = NULL;
                terrain_list[temp_id] = NULL;
            }
            Terrain* new_terrain = NULL;
            switch(inTT) {
                case TERRAIN_TYPE_EARTH:
                    new_terrain = new Terrain_Earth();
                    break;
            }
            if(new_terrain) {
                new_terrain->x = x;
                new_terrain->y = y;
                new_terrain->id = this_id;
            }
            terrain_grid[x][y] = new_terrain;
            terrain_list[this_id] = new_terrain;
            this_id++;
        }
    }
}

void Environment::fillTerrain(TerrainType inTT, int in_w, int in_h, int in_x, int in_y) {
    if(in_x<0) in_x = 0;
    if(in_y<0) in_y = 0;

    for(int x = in_x;  (x<(in_x+in_w))&&(x < w); x++) {
        for(int y = in_y; (y<(in_y+in_h))&&(y < h); y++) {
            if(terrain_grid[x][y]) {
                int temp_id = terrain_grid[x][y]->id;
                delete terrain_grid[x][y];
                terrain_grid[x][y] = NULL;
                terrain_list[temp_id] = NULL;
            }
            Terrain* new_terrain = NULL;
            switch(inTT) {
                case TERRAIN_TYPE_EARTH:
                    new_terrain = new Terrain_Earth();
                    break;
            }
            int this_id=-1;
            if(new_terrain) {
                for(int i = 0; (this_id==-1)&&(i < max_terrain); i++) {
                    if(!terrain_list[i]) this_id = i;
                }
                new_terrain->x = x;
                new_terrain->y = y;
                new_terrain->id = this_id;
                terrain_list[this_id] = new_terrain;
            }
            terrain_grid[x][y] = new_terrain;
        }
    }
}

int Environment::spawnLives(double in_P,int in_r, int in_g, int in_b,Gender in_gender, int d_rgb) {
    int out = 0;
    for(int x = 0; x < w; x++) {
        for(int y = 0; y < h; y++) {
            if(Utils::rand_d()<in_P) {
                int this_ind = spawnLife(in_r,in_g,in_b,in_gender,x,y,d_rgb);
                if(this_ind!=-1) out++;
            }
        }
    }
    return out;
}

bool Environment::moveLife(int life_ind, int dx, int dy) {
    bool out = 0;

    Life* this_life = life_list[life_ind];
    if(this_life) {
        int x = this_life->x;
        int y = this_life->y;
        int xp = x+dx;
        int yp = y+dy;
        while(xp<0) xp+=w;
        while(yp<0) yp+=h;
        xp%=w;
        yp%=h;
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

void Environment::step(bool once) {
    // Herbivores
    if(once||((period>0)&&(!(time%period)))) {
        for(int i = 0; i < max_life; i++) {
            Life* this_life = life_list[i];
            if(this_life) {
                updateLife(this_life);
            }
        }
    }
    time++;
}

void Environment::updateLife(Life* in_l) {
    // Die
    if(in_l) {
        int this_x = in_l->x;
        int this_y = in_l->y;
        int this_id = in_l->id;
        if((!terrain_grid[this_x][this_y])||(!terrain_grid[this_x][this_y]->occupiable)) {
            delete life_grid[this_x][this_y];
            life_grid[this_x][this_y] = NULL;
            life_list[this_id] = NULL;
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

void Environment::printLifeAt(int in_x, int in_y) const {
    if((in_x>=0)&&(in_y>=0)&&(in_x<w)&&(in_y<h)) {
        if(life_grid[in_x][in_y]) {
            life_grid[in_x][in_y]->print();
        } else {
            cout << "No life here." << endl;
        }
    }
}
