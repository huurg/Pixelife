#ifndef LC_RENDER_OBJECT_HEADER
#define LC_RENDER_OBJECT_HEADER

#include <SDL.h>

class LC_RenderObject {
    protected:
        SDL_Renderer* renderer = NULL;
        LC_RenderObject* parent = NULL;
    public:
        int screen_width = 0;
        int screen_height = 0;
        int screen_x_pos = 0;
        int screen_y_pos = 0;
        double world_width = 1.0;
        double camera_x_pos = 0.0;
        double camera_y_pos = 0.0;
        double camera_zoom = 1.0;

        virtual void render();
        virtual void setRenderer(SDL_Renderer* ir);
        void setParent(LC_RenderObject* iro);

        void coordsScreenToWorld(double* ox = NULL, double* oy = NULL, double* ow = NULL, double* oh = NULL) const;
        void coordsScreenToWorld(int* ox = NULL, int* oy = NULL, int* ow = NULL, int* oh = NULL) const;
        void coordsWorldToScreen(int* ox = NULL, int* oy = NULL, int* ow = NULL, int* oh = NULL) const;
        void coordsMouseToScreen(int* ox = NULL, int* oy = NULL, int* ow = NULL, int* oh = NULL) const;
        void coordsScreenToMouse(int* ox = NULL, int* oy = NULL, int* ow = NULL, int* oh = NULL) const;
        void moveCamera(int dx, int dy, double dz);
};

#endif // LC_RENDER_OBJECT_HEADER
