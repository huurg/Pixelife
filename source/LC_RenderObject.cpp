#include "LC_RenderObject.h"

#include <SDL.h>

#include <cmath>

void LC_RenderObject::render() {
    ;
}

void LC_RenderObject::setRenderer(SDL_Renderer* ir) {
    renderer = ir;
}

void LC_RenderObject::setParent(LC_RenderObject* iro) {
    parent = iro;
}

void LC_RenderObject::coordsScreenToWorld(double* ox, double* oy, double* ow, double* oh) const {
    if(ox) {
        double this_x = *ox;
        this_x = (world_width*(this_x)/(double)(camera_zoom*screen_width)+camera_x_pos);
        *ox = this_x;
    }
    if(oy) {
        double this_y = *oy;
        this_y = (world_width*(this_y)/(double)(camera_zoom*screen_width)+camera_y_pos);
        *oy = this_y;
    }
    if(ow) {
        double this_w = *ow;
        this_w = world_width*(this_w)/(double)(camera_zoom*screen_width);
        *ow = this_w;
    }
    if(oh) {
        double this_h = *oh;
        this_h = world_width*(this_h)/(double)(camera_zoom*screen_width);
        *oh = this_h;
    }
}

void LC_RenderObject::coordsWorldToScreen(int* ox, int* oy, int* ow, int* oh) const {
    if(ox) {
        int this_x = *ox;
        this_x = camera_zoom*screen_width*(this_x-camera_x_pos)/(double)world_width;
        *ox = this_x;
    }
    if(oy) {
        int this_y = *oy;
        this_y = camera_zoom*screen_width*(this_y-camera_y_pos)/(double)world_width;
        *oy = this_y;
    }
    if(ow) {
        int this_w = *ow;
        this_w = ceil(camera_zoom*screen_width*(this_w)/(double)world_width);
        *ow = this_w;
    }
    if(oh) {
        int this_h = *oh;
        this_h = ceil(camera_zoom*screen_width*(this_h)/(double)world_width);
        *oh = this_h;
    }
}

void LC_RenderObject::coordsMouseToScreen(int* ox, int* oy, int* ow, int* oh) const {
    if(ox) {
        int this_x = *ox;
        this_x = this_x - screen_x_pos;
        *ox = this_x;
    }
    if(oy) {
        int this_y = *oy;
        this_y = this_y - screen_y_pos;
        *oy = this_y;
    }
    if(ow) {
        int this_w = *ow;
        this_w = this_w;
        *ow = this_w;
    }
    if(oh) {
        int this_h = *oh;
        this_h = this_h;
        *oh = this_h;
    }
}

void LC_RenderObject::coordsScreenToMouse(int* ox, int* oy, int* ow, int* oh) const {
    if(ox) {
        int this_x = *ox;
        this_x = this_x + screen_x_pos;
        *ox = this_x;
    }
    if(oy) {
        int this_y = *oy;
        this_y = this_y + screen_y_pos;
        *oy = this_y;
    }
    if(ow) {
        int this_w = *ow;
        this_w = this_w;
        *ow = this_w;
    }
    if(oh) {
        int this_h = *oh;
        this_h = this_h;
        *oh = this_h;
    }
}

void LC_RenderObject::coordsScreenToWorld(int* ox, int* oy, int* ow, int* oh) const {
    double oxd = *ox;
    double oyd = *oy;
    double owd = *ow;
    double ohd = *oh;
    coordsScreenToWorld(&oxd,&oyd,&owd,&ohd);
    *ox = oxd;
    *oy = oyd;
    *ow = owd;
    *oh = ohd;
}

void LC_RenderObject::moveCamera(int dx, int dy, double dz) {
    camera_x_pos += dx;
    camera_y_pos += dy;
    if((camera_zoom+dz)>(world_width/(double)screen_width))
    camera_zoom += dz;
}
