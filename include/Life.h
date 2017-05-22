#ifndef LIFE_INCLUDE
#define LIFE_INCLUDE

#include "EnvironmentObject.h"
#include "Life_Action.h"

const int LIFE_DEFAULT_ENERGY = 100;
const int LIFE_DEFAULT_STAMINA = 100;

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
    LIFE_DO_STATE_NULL,
    LIFE_DO_STATE_IDLE,
    LIFE_DO_STATE_EAT,
    LIFE_DO_STATE_WALK,
    LIFE_DO_STATE_RUN,
    LIFE_DO_STATE_SEX,
    LIFE_DO_STATE_GESTATE,
    LIFE_DO_STATE_GIVE_BIRTH,
    LIFE_DO_STATE_FLOWER,
    LIFE_DO_STATE_GROW_SEED,
    LIFE_DO_STATE_DISTRIBUTE_SEED,
    LIFE_DO_STATE_ATTACK,
    LIFE_DO_STATE_DIE
};

struct Life : public EnvironmentObject {
    int energy = LIFE_DEFAULT_ENERGY;
    Gender gender = GENDER_NULL;
    LifeType type = LIFE_TYPE_NULL;
    int stamina = LIFE_DEFAULT_STAMINA;
    int stamina_max = LIFE_DEFAULT_STAMINA;
    Life_Action idle;
    Life_Action eat;
    int speed = 0;
    int line_of_sight = 0;

    bool v_r;
    bool v_g;
    bool v_b;

    int w_r;
    int w_g;
    int w_b;

    bool can_die;
    bool can_move;
    bool can_eat_herbivores;
    bool can_eat_sun;
    bool can_eat_plants;
    bool can_reproduce_sexually;
    bool can_reproduce_asexually;
    bool can_be_eaten_alive;

    Life_Action walk;
    Life_Action run;
    int move_direction_x = 0;
    int move_direction_y = 0;
    double probability_direction_change = 0.15;
    double probability_idle = 0.5;

    Life_Action sex;
    Life_Action gestate;
    Life_Action birth;
    Life* child = NULL;

    Life_Action flower;
    Life_Action grow_seed;
    Life_Action distribute_seed;
    int seeds = 0;

    Life();
    Life(Gender in_gender);
    Life(int in_r, int in_g, int in_b, Gender in_a = GENDER_NULL);
    ~Life();

    void setColour(int in_r, int in_g, int in_b);
    void varyColour(int d_rgb);
    void print() const;
};

#endif // LIFE_INCLUDE
