#ifndef FRACTAL_EXPLORER_RENDER_H
#define FRACTAL_EXPLORER_RENDER_H

#include "cl_context.h"
#include "fractal.h"
#include "camera.h"


class Render {
public:
    Render(CLContext & cl_context, cl_float3 sun_direction, Fractal & fractal);

    Camera & get_camera() { return camera; }

    void render(cl::Image2D & image, int width, int height);

private:
    CLContext & context;
    Fractal & fractal;
    Camera camera;

    cl::Program program;
    cl::Program::Sources sources;
    cl::Kernel render_kernel;

};


#endif