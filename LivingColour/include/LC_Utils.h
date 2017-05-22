#ifndef LC_UTILS_HEADER
#define LC_UTILS_HEADER

#include <iostream>
using namespace std;

#include <SDL.h>

namespace LC_Utils {
    SDL_Texture* LC_LoadTexture(SDL_Renderer* in_renderer, const char* in_filename, int r = 0, int g = 0, int b = 0, int a = 255);
}

#endif // LC_UTILS_HEADER
