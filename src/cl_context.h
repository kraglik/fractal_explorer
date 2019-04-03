#ifndef FRACTAL_EXPLORER_CL_CONTEXT_H
#define FRACTAL_EXPLORER_CL_CONTEXT_H

#define __CL_ENABLE_EXCEPTIONS
#ifdef __APPLE__
#include <OpenCL/cl.hpp>
#else
#include <CL/cl.hpp>
#endif
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

class CLContext {
    public:
        CLContext() = default;
        explicit CLContext(SDL_GLContext gl_context);

        cl::Platform & get_platform() { return platform; }
        cl::Device & get_device() { return device; }
        cl::Context & get_context() { return context; }
        cl::CommandQueue & get_queue() { return queue; }

    private:
        cl::Platform platform;
        cl::Device device;
        cl::Context context;
        cl::CommandQueue queue;
};


#endif