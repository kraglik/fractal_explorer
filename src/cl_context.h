#ifndef FRACTAL_EXPLORER_CL_CONTEXT_H
#define FRACTAL_EXPLORER_CL_CONTEXT_H

#ifdef __APPLE__
#include <OpenCL/cl.hpp>
#else
#include <CL/cl.hpp>
#endif

class CLContext {
    public:
        CLContext();

        cl::Platform get_platform();
        cl::Device get_device();
        cl::Context get_context();
        cl::CommandQueue get_queue();

    private:
        cl::Platform platform;
        cl::Device device;
        cl::Context context;
        cl::CommandQueue queue;
};


#endif