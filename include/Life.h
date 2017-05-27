#ifndef LIFE_INCLUDE
#define LIFE_INCLUDE

#include "EnvironmentObject.h"
#include "Life_Action.h"

const int LIFE_DEFAULT_ENERGY = 300;
const int LIFE_DEFAULT_STAMINA = 100;
const double LIFE_MUTATION_P_INIT = 0.2;
const double LIFE_MUTATION_P_FINAL = 0.01;
const int LIFE_N_ACTIONS = 12;

enum LifeType {
    LIFE_TYPE_NULL,
    LIFE_TYPE_GRASS,
    LIFE_TYPE_TREE,
    LIFE_TYPE_HERBIVORE,
    LIFE_TYPE_PREDATOR
};

enum Gender {
    GENDER_NULL,
    GENDER_FEMALE,
    GENDER_MALE
};

enum LifeDoState {
    LIFE_DO_STATE_NULL=-1,
    LIFE_DO_STATE_IDLE=0,
    LIFE_DO_STATE_EAT=1,
    LIFE_DO_STATE_DECAY=2,
    LIFE_DO_STATE_WALK=3,
    LIFE_DO_STATE_RUN=4,
    LIFE_DO_STATE_MATE=5,
    LIFE_DO_STATE_GESTATE=6,
    LIFE_DO_STATE_GIVE_BIRTH=7,
    LIFE_DO_STATE_FLOWER=8,
    LIFE_DO_STATE_GROW_SEED=9,
    LIFE_DO_STATE_DISTRIBUTE_SEED=10,
    LIFE_DO_STATE_REST=11
    //LIFE_DO_STATE_ATTACK,
    //LIFE_DO_STATE_DIE
};

struct Life : public EnvironmentObject {
    bool alive = 1;
    double energy = LIFE_DEFAULT_ENERGY;
    Gender gender = GENDER_NULL;
    LifeType type = LIFE_TYPE_NULL;
    double stamina = LIFE_DEFAULT_STAMINA;
    double stamina_max = LIFE_DEFAULT_STAMINA;//
    Life_Action idle;//
    Life_Action rest;
    Life_Action eat;//
    double eat_efficiency = 1.0; // Used for external factors e.g. sun
    double eat_cap = -1.0;
    double probability_ignore_food = 0.5; //
    Life_Action decay;//
    int speed = 0;//
    int line_of_sight = 0;//
    LifeDoState do_state = LIFE_DO_STATE_IDLE;

    bool v_r;
    bool v_g;
    bool v_b;

    int w_r;
    int w_g;
    int w_b;

    bool chromenome[24];

    bool can_do = 1;

    bool can_die;
    bool can_move;
    bool can_eat_herbivores;
    bool can_eat_sun;
    bool can_eat_plants;
    bool can_reproduce_sexually;
    bool can_reproduce_asexually;
    bool can_be_eaten_alive;

    Life_Action walk;//*4
    Life_Action run;//*4
    int move_direction_x = 0;
    int move_direction_y = 0;
    double probability_direction_change = 0.15;//
    double probability_idle = 0.5;//
    double probability_notice_prey = 0.6;//
    double probability_notice_pred = 0.8;//

    Life_Action mate;//*4
    Life_Action gestate;//*4
    Life_Action birth;//*4
    Life* child = NULL;
    double mating_constant = 1.0;//
    int birth_energy = 100; // Upon birth only
    double pregnancy_probability = 0.0;

    Life_Action flower;//*4
    Life_Action grow_seed;//*4
    Life_Action distribute_seed;//*4
    Life* parent = NULL;
    int flower_range = 10; //
    int seed_range = 10; //
    int seeds = 0;
    int seed_energy = 100; // Per seed cost
    double probability_grow = 0.05;
    double probability_distribute = 0.05;

    double probability_notice_partner = 0.5; //
    Life_Action* life_actions[LIFE_N_ACTIONS];

    Life();
    Life(Gender in_gender);
    Life(int in_r, int in_g, int in_b, Gender in_a = GENDER_NULL);
    ~Life();

    void setColour(int in_r, int in_g, int in_b);
    void varyColour(int d_rgb);
    void print() const;

    void setDefaultParameters();

    void randomizeDirection();

    Life_Action* getCurrentAction();

    bool act(LifeDoState ids, bool force = 0); // set do state and set time = duration
    void tick();// Update counters for action cooldown and time as well as decrement energy and life according to costs/duration

    static int eatScore(const Life* lifeA, const Life* lifeB);
    static double eatProbability(const Life* lifeA, const Life* lifeB);
    static double mateProbability(const Life* lifeA, const Life* lifeB);
    static void ChromenomeToRGB(bool* in_chr, int* oR, int* oG, int* oB);
    static void RGBToChromenome(int* iR, int* iG, int* iB, bool* oCHR);
    static void MixChromenes(bool* iA, bool* iB, bool* oC);
    static Life* MixChromenes(Life* iA, Life* iB);
};

#endif // LIFE_INCLUDE
