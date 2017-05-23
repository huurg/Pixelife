#ifndef LC_IMAGE_HEADER
#define LC_IMAGE_HEADER

#include "LC_RenderObject.h"
#include <SDL.h>

struct LC_Image : public LC_RenderObject {
    SDL_Color bg_colour;
    SDL_Color border_colour;
    SDL_Rect image_region;
    bool bg_fill = 1;
    bool border_draw = 0;
    bool colour_keying = 1;
    Uint8 texture_alpha = 255;
    private:
        SDL_Texture* texture = NULL;
        SDL_Surface* surface = NULL;
        SDL_Color key_colour;
    public:
        LC_Image();
        ~LC_Image();
        void render();
        void loadImage(const char* in_filename);
        void refresh();
        void setKeyColour(int r, int g, int b);
        void setRenderer(SDL_Renderer* ir);
};

#endif // LC_IMAGE_HEADER
