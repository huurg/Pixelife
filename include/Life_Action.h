#ifndef LIFE_ACTION_INCLUDE
#define LIFE_ACTION_INCLUDE

struct Life_Action {
    double stamina_cost = 0;
    double energy_cost = 0;
    int cooldown_max = 0;
    int cooldown = 0;
    int duration = 0;
    int time = 0;
    bool action_end = 0; //Only set to 1 for first frame that time goes to 0 (by tick).
    bool background = 0;

    Life_Action();
};

#endif // LIFE_ACTION_INCLUDE
