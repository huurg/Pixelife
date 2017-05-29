#include "Life.h"

#include <SDL.h>
#include <iostream>
using namespace std;

#include "Utils.h"

#include <cstdlib>
#include <cmath>

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
    can_die = (v_r||v_g||v_b)&&!(v_r&&v_g&&v_b);
    can_move = (!(v_r&&v_g&&v_b))&&((!v_g)||(v_g&&((v_r&&(w_r>w_g)))||(v_b&&(w_b>w_g))));
    can_reproduce_sexually = (v_r||v_b)&&(!v_g);
    can_reproduce_asexually = (!(v_r&&v_g&&v_b))&&(!can_move);
    can_be_eaten_alive = v_g;
    RGBToChromenome(&in_r, &in_g, &in_b, chromenome);
}

Life::Life(int in_r, int in_g, int in_b, Gender in_a) {
    setColour(in_r,in_g,in_b);
    if(in_a==GENDER_NULL) {
        if(Utils::rand_d()<0.5) gender = GENDER_FEMALE; else gender = GENDER_MALE;
    } else {
        gender = in_a;
    }

    life_actions[LIFE_DO_STATE_IDLE] = &idle;
    life_actions[LIFE_DO_STATE_EAT] = &eat;
    life_actions[LIFE_DO_STATE_DECAY] = &decay;
    life_actions[LIFE_DO_STATE_WALK] = &walk;
    life_actions[LIFE_DO_STATE_RUN] = &run;
    life_actions[LIFE_DO_STATE_MATE] = &mate;
    life_actions[LIFE_DO_STATE_GESTATE] = &gestate;
    life_actions[LIFE_DO_STATE_GIVE_BIRTH] = &birth;
    life_actions[LIFE_DO_STATE_FLOWER] = &flower;
    life_actions[LIFE_DO_STATE_GROW_SEED] = &grow_seed;
    life_actions[LIFE_DO_STATE_DISTRIBUTE_SEED] = &distribute_seed;
    life_actions[LIFE_DO_STATE_REST] = &rest;

    setDefaultParameters();

    idle.time = idle.duration;
}

Life::Life() {
    Life(rand()%256,rand()%256,rand()%256);
}

Life::Life(Gender in_gender) {
    setColour(rand()%256,rand()%256,rand()%256);
    gender = in_gender;
}

Life::~Life() {
    if(child) delete child;
    if(parent) delete parent;
}

void Life::print() const {
    cout << "Life object." << endl;
    cout << "\tCan die = " << can_die << endl;
    cout << "\tposition = (" << x << ", " << y << ")" << endl;
    cout << "\tgender = ";
    if(gender==GENDER_FEMALE) cout << "female" << endl;
    if(gender==GENDER_MALE) cout << "male" << endl;
    cout << "\tage = " << age << endl;
    cout << "\trgb = ("  << (int)colour.r << ", " << (int)colour.g << ", " << (int)colour.b << ")" << endl;
    cout << "\tv = (" << v_r << ", " << v_g << ", " << v_b << ")" << endl;
    cout << "\tw = (" << w_r << ", " << w_g << ", " << w_b << ")" << endl;
    cout << "\tChromenome: ";
    for(int i = 0; i < 24; i++) cout << chromenome[i];
    cout << endl;
    cout << "\tEnergy = " << energy << endl;
    cout << "\tStamina = " << stamina << endl;
    cout << "\tCurrently doing: ";
    switch(do_state) {
        case LIFE_DO_STATE_IDLE:
            cout << "Idling";
            break;
        case LIFE_DO_STATE_EAT:
            cout << "Eating";
            break;
        case LIFE_DO_STATE_DECAY:
            cout << "Decaying";
            break;
        case LIFE_DO_STATE_WALK:
            cout << "Walking";
            break;
        case LIFE_DO_STATE_RUN:
            cout << "Running";
            break;
        case LIFE_DO_STATE_MATE:
            cout << "Mating";
            break;
        case LIFE_DO_STATE_GESTATE:
            cout << "Gestating";
            break;
        case LIFE_DO_STATE_GIVE_BIRTH:
            cout << "Giving birth";
            break;
        case LIFE_DO_STATE_FLOWER:
            cout << "Flowering";
            break;
        case LIFE_DO_STATE_GROW_SEED:
            cout << "Growing seed";
            break;
        case LIFE_DO_STATE_DISTRIBUTE_SEED:
            cout << "Distributing seed";
            break;
        case LIFE_DO_STATE_REST:
            cout << "Resting";
            break;
        default:
            cout << "Unknown";
            break;
    }
    cout << endl;
    cout << "\tCan produce sexually = " << can_reproduce_sexually << endl;
    cout << "\tCan produce asexually = " << can_reproduce_asexually << endl;
    if(can_reproduce_asexually) cout << "\tSeeds = " << seeds << endl;
    if(child) {
        cout << "\tWith child:" << endl;
        child->print();
    }
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

int Life::eatScore(const Life* lifeA, const Life* lifeB) {
    int out = 0;

    if(lifeA&&lifeB) {
        out = lifeA->v_r*lifeA->w_r*lifeB->v_b*lifeB->w_b + lifeA->v_b*lifeA->w_b*lifeB->v_g*lifeB->w_g;
    }

    return out;
}

double Life::eatProbability(const Life* lifeA, const Life* lifeB) {
    double out = 0.0;

    double E_AB = eatScore(lifeA, lifeB);
    double E_BA = eatScore(lifeB, lifeA);

    if((E_AB==0.0)&&(E_BA==0.0)) {
        out = 0.0;
    } else {
        out = 0.5*((E_AB-E_BA)/(E_AB+E_BA)+1.0);

        if(lifeA->can_eat_herbivores&&lifeB->alive&&!lifeB->can_be_eaten_alive) {
            if((lifeA->stamina/lifeA->stamina_max>0.0)||(lifeB->stamina/lifeB->stamina_max>0.0)) {
                out *= 0.5*((lifeA->stamina/lifeA->stamina_max-lifeB->stamina/lifeB->stamina_max)/(lifeA->stamina/lifeA->stamina_max+lifeB->stamina/lifeB->stamina_max)+1.0);
                if((lifeA->energy>0.0)||(lifeB->energy>0.0)) {
                    out *= 0.5*((lifeA->energy-lifeB->energy)/(lifeA->energy+lifeB->energy)+1.0);
                } else {
                    out = 0.0;
                }
            } else {
                out = 0.0;
            }
        }

    }


    return out;
}

void Life::setDefaultParameters() {
    stamina_max = 100;

    idle.cooldown_max = 1;
    idle.duration = 1;
    idle.energy_cost = 0.1;
    idle.stamina_cost = 0.1;
    resume_probability = 1.0;

    rest.cooldown_max = (int)stamina_max;
    rest.duration = (int)stamina_max;
    rest.energy_cost = 0.01;
    rest.stamina_cost = -(int)stamina_max;

    eat.cooldown_max = 30;
    eat.duration = 30;
    eat.energy_cost = -100;
    eat.stamina_cost = 10;

    probability_ignore_food = 0.5;
    if(can_eat_herbivores) {
        probability_ignore_food = 0.001;
        eat.cooldown_max = 2000 ;
        eat.energy_cost = -500;
        eat.duration = 50;
        if(can_eat_plants) {
            eat.energy_cost = -250;
        }
        if(can_eat_sun) {
            eat.energy_cost = -100;
        }
    }

    if(can_eat_sun&&!(can_eat_herbivores)) {
        eat.cooldown_max = 1;
        eat.duration = 1;
        eat.energy_cost = -2;
        eat.stamina_cost = 0.1;
        eat.background = 1;
    }

    decay.cooldown_max = 1;
    decay.duration = 1;
    decay.energy_cost = 1;
    decay.stamina_cost = 1;

    speed = 0;
    line_of_sight = 5;

    walk.cooldown_max = 1;//10; Switch back to this after run implemented
    walk.duration = 1;
    walk.energy_cost = 1;
    walk.stamina_cost = 1;

    run.cooldown_max = 2;
    run.duration = 1;
    run.energy_cost = 10;
    run.stamina_cost = 10;

    probability_direction_change = 0.4;
    probability_idle = 0.2;
    probability_notice_prey = 0.6;
    probability_notice_pred = 0.8;

    if(can_eat_herbivores) {
        probability_direction_change = 0.05;
        probability_idle = 0.8;
    }

    if(!(v_r||v_g||v_b)) {
        probability_direction_change = 0.15;
        probability_idle = 0.0;
        rest.duration = 1;
    }


    mate.cooldown_max = 30;
    mate.duration = 30;
    mate.energy_cost = 5;//100;
    mate.stamina_cost = 5;//100;

    gestate.cooldown_max = 300;
    gestate.duration = 900;
    gestate.energy_cost = 300;//100;
    gestate.stamina_cost = 30;//100;
    gestate.background = 1;

    birth.cooldown_max = 300;
    birth.duration = 100;
    birth.energy_cost = 20;//50;
    birth.stamina_cost = 10;//50;
    mating_constant = 1.0;
    birth_energy = 100;

    flower.cooldown_max = 300;
    flower.duration = 300;
    flower.energy_cost = 40;//100;
    flower.stamina_cost = 40;//100;

    grow_seed.cooldown_max = 300;
    grow_seed.duration = 300;
    grow_seed.energy_cost = 50;//500;
    grow_seed.stamina_cost = 50;//500;

    distribute_seed.cooldown_max = 300;
    distribute_seed.duration = 300;
    distribute_seed.energy_cost = 30;//100;
    distribute_seed.stamina_cost = 300;//100;
    flower_range = 10;
    seed_range = 3;
    seed_energy = 100;
    probability_grow = 0.5;
    probability_distribute = 0.5;

    probability_notice_partner = 0.5;
    if(can_reproduce_asexually) {
        probability_notice_partner = 0.05;
    }
}

void Life::RGBToChromenome(int* iR, int* iG, int* iB, bool* oCHR) {
    for(int i = 0; i < 8; i++) {
        oCHR[3*i] = Utils::Bn(*iB,i);
        oCHR[3*i+1] = Utils::Bn(*iG,i);
        oCHR[3*i+2] = Utils::Bn(*iR,i);
    }
}

void Life::ChromenomeToRGB(bool* iCHR, int* oR, int* oG, int* oB) {
    int this_r = 0;
    int this_g = 0;
    int this_b = 0;
    for(int i = 0; i < 8; i++) {
        this_b += iCHR[3*i]*pow(2,i);
        this_g += iCHR[3*i+1]*pow(2,i);
        this_r += iCHR[3*i+2]*pow(2,i);
    }
    *oR = this_r;
    *oG = this_g;
    *oB = this_b;
}

void Life::MixChromenes(bool* iA, bool* iB, bool* oC) {
    for(int i = 0; i < 8; i++) {
        for(int j = 0; j < 3; j++) {
            int I = 3*i + j;
            if(Utils::rand_d()<(LIFE_MUTATION_P_INIT/pow(pow(LIFE_MUTATION_P_INIT/LIFE_MUTATION_P_FINAL,1.0/23),i))) {
                //cout << "Mutation! i = " << i << "; P(i) = " << (LIFE_MUTATION_P_INIT/pow(pow(LIFE_MUTATION_P_INIT/LIFE_MUTATION_P_FINAL,1.0/23),i)) << endl;
                oC[I] = (Utils::rand_d()<0.5);
            } else {
                if(iA[I]==iB[I]) {
                    oC[I] = iA[I];
                } else {
                    oC[I] = (Utils::rand_d()<0.5);
                }
            }
        }
    }
}

Life* Life::MixChromenes(Life* iA, Life* iB) {
    Life* out = NULL;
    bool new_chr[24];
    int this_r, this_g, this_b;
    MixChromenes(iA->chromenome,iB->chromenome,new_chr);
    ChromenomeToRGB(new_chr,&this_r,&this_g,&this_b);
    out = new Life(this_r,this_g,this_b);
    return out;
}

void Life::randomizeDirection() {
    if(Utils::rand_d()<probability_direction_change) {
        if(Utils::rand_d()<0.2) {
            move_direction_x = 0.0;
            move_direction_y = 0.0;
        } else {
            move_direction_x = 1.0;
            move_direction_y = 0.0;
            if(Utils::rand_d()<0.5) {
                move_direction_x = 1.0 - move_direction_x;
                move_direction_y = 1.0 - move_direction_y;
            }
            if(Utils::rand_d()<0.5) {
                move_direction_x *= -1.0;
                move_direction_y *= -1.0;
            }
        }
    }
}

double Life::mateProbability(const Life* lifeA, const Life* lifeB) {
    double out = 0.0;

    double this_d = pow(fabs(pow(lifeA->colour.r-lifeB->colour.r,2.0))+fabs(pow(lifeA->colour.g-lifeB->colour.g,2.0))+fabs(pow(lifeA->colour.b-lifeB->colour.b,2.0)),0.5);
    //cout << "this_d = " << (this_d/255.0) << endl;
    //cout << "out = " << out << endl;
    out = 1.0-pow(100,-lifeA->mating_constant*lifeB->mating_constant*this_d/255.0);
    //out *= (lifeA->v_r&&lifeB->v_r)||(lifeA->v_g&&lifeB->v_g)||(lifeA->v_b&&lifeB->v_b);
    out *= lifeA->alive&&lifeB->alive;
    out *= (lifeA->can_reproduce_sexually&&lifeB->can_reproduce_sexually&&
            (((lifeA->gender==GENDER_FEMALE)&&(lifeB->gender==GENDER_MALE))||
             ((lifeA->gender==GENDER_MALE)&&(lifeB->gender==GENDER_FEMALE))))||
             (lifeA->can_reproduce_asexually&&lifeB->can_reproduce_asexually);

    return out;
}

bool Life::act(LifeDoState ids, bool force) {
    bool out = 0;
    Life_Action* this_action = life_actions[ids];
    Life_Action* current_action = life_actions[do_state];
    if(force) {
        this_action->cooldown = 0;
        //current_action->time = 0;
        //current_action->action_end = 0;
    }
    if((this_action->cooldown==0)/*&&(current_action->time==0)&&(current_action->action_end==0)*/) {
        do_state = ids;
        this_action->time = this_action->duration;
        out = 1;
    }
    return out;
}

void Life::tick() {
    Life_Action* this_action = NULL;
    for(int i = 0; i < LIFE_N_ACTIONS; i++) {
        this_action = life_actions[i];
        if(this_action->cooldown>0) this_action->cooldown--;
        if((do_state==i)||(this_action->background)) {
            if(this_action->time>0) {
                if(i==LIFE_DO_STATE_EAT) {
                    double this_dE = eat_efficiency*(double)this_action->energy_cost/(double)this_action->duration;
                    if((eat_cap>0.0)&&(fabs(this_dE)>(eat_cap/(double)this_action->duration))) this_dE = -eat_cap/(eat_cap/(double)this_action->duration);
                    energy -= this_dE;
                } else {
                    energy -= (double)this_action->energy_cost/(double)this_action->duration;
                }
                stamina -= (double)this_action->stamina_cost/(double)this_action->duration;
                if(energy < 0.0) energy = 0.0;
                if(stamina < 0.0) stamina = 0.0;
                this_action->time--;
                if(this_action->time==0) {
                    this_action->action_end = 1;
                    this_action->cooldown = this_action->cooldown_max;
                    //if(i==LIFE_DO_STATE_GESTATE) cout << "WILL TRY TO BIRTH" << endl;
                }
            }
        }
    }
    if(stamina > 0.0) can_do = 1;
    else can_do = 0;
    if(stamina > stamina_max) stamina = stamina_max;
    age++;
}

Life_Action* Life::getCurrentAction() {
    return life_actions[do_state];
}

void Life::kill() {
    alive = 0;
    stamina = 0;
    for(int i = 0; i < LIFE_N_ACTIONS; i++) {
        life_actions[i]->time = 0;
        life_actions[i]->action_end = 0;
        life_actions[i]->cooldown = 0;
    }
    act(LIFE_DO_STATE_DECAY,true);
}
