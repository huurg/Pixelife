#include "Life.h"

#include <SDL.h>
#include <iostream>
using namespace std;

#include "Utils.h"

#include <cstdlib>

void Life::setColour(int in_r, int in_g, int in_b) {
    colour.r = (Uint8) in_r;
    colour.g = (Uint8) in_g;
    colour.b = (Uint8) in_b;
    colour.a = (Uint8) 255;
    v_r = (in_r >= 128);
    v_g = (in_g >= 128);
    v_b = (in_b >= 128);
    w_r = (in_r%128);
    w_g = (in_g%128);
    w_b = (in_b%128);
    can_eat_herbivores = v_r;
    can_eat_sun = v_g;
    can_eat_plants = v_b;
    can_die = v_r||v_g||v_b;
    can_move = (!(v_r&&v_g&&v_b))&&((!v_g)||(v_g&&((v_r&&(w_r>w_g)))||(v_b&&(w_b>w_g))));
    can_reproduce_sexually = (!(!v_r)&&(!v_g)&&(!v_b))&&can_move;
    can_reproduce_asexually = (!(v_r&&v_g&&v_b))&&(!can_move);
    can_be_eaten_alive = v_g;
}

Life::Life(int in_r, int in_g, int in_b, Gender in_a) {
    setColour(in_r,in_g,in_b);
    if(in_a==GENDER_NULL) {
        if(Utils::rand_d()<0.5) gender = GENDER_FEMALE; else gender = GENDER_MALE;
    } else {
        gender = in_a;
    }
}

Life::Life() {
    Life(rand()%256,rand()%256,rand()%256);
}

Life::Life(Gender in_gender) {
    setColour(rand()%256,rand()%256,rand()%256);
    gender = in_gender;
}

Life::~Life() {
    ;
}

void Life::print() const {
    cout << "Life object." << endl;
    cout << "\tposition = (" << x << ", " << y << ")" << endl;
    cout << "\tgender = ";
    if(gender==GENDER_FEMALE) cout << "female" << endl;
    if(gender==GENDER_MALE) cout << "male" << endl;
    cout << "\trgb = ("  << (int)colour.r << ", " << (int)colour.g << ", " << (int)colour.b << ")" << endl;
    cout << "\tv = (" << v_r << ", " << v_g << ", " << v_b << ")" << endl;
    cout << "\tw = (" << w_r << ", " << w_g << ", " << w_b << ")" << endl;
}

void Life::varyColour(int d_rgb) {
    int rp = (int)colour.r + (rand()%(2*d_rgb+1)) - d_rgb;
    int gp = (int)colour.g + (rand()%(2*d_rgb+1)) - d_rgb;
    int bp = (int)colour.b + (rand()%(2*d_rgb+1)) - d_rgb;
    if(rp<0) rp = 0;
    if(gp<0) gp = 0;
    if(bp<0) bp = 0;
    if(rp>255) rp = 255;
    if(gp>255) gp = 255;
    if(bp>255) bp = 255;
    setColour(rp,gp,bp);
}
