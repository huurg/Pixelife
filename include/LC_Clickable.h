#ifndef LC_CLICKABLE_HEADER
#define LC_CLICKABLE_HEADER

#include <SDL.h>

enum LC_ClickableState {
    LC_CLICKABLE_STATE_OVER,
    LC_CLICKABLE_STATE_DOWN,
    LC_CLICKABLE_STATE_UP,
    LC_CLICKABLE_STATE_N
};

struct LC_Clickable {
    SDL_Rect click_region;
    LC_ClickableState click_state = LC_CLICKABLE_STATE_UP;
    void (*mouseOverAction)(void) = NULL;
    void (*mouseDownAction)(void) = NULL;
    void (*mouseUpAction)(void) = NULL;
};

#endif // LC_CLICKABLE_HEADER
