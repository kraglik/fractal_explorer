#include "render.h"
#include <exception>
#include <iostream>

Render::Render(CLContext &cl_context, cl_float3 sun_direction, Fractal &fractal)
    : context(cl_context), fractal(fractal), camera(fractal.get_initial_camera()) {

    sources = cl::Program::Sources();

    std::string & kernel_code = fractal.get_kernel();

    sources.push_back({kernel_code.c_str(), kernel_code.length()});
    program = cl::Program(cl_context.get_context(), sources);

    if (program.build({cl_context.get_device()}) != CL_SUCCESS) {
        throw * new std::runtime_error("Building kernel failed.");
    }

    render_kernel = cl::Kernel(program, "render");
}

void Render::render(cl::Image2D &image, int width, int height) {

    std::vector<cl::Memory> gl_objects = {image};

    context.get_queue().enqueueAcquireGLObjects(&gl_objects);
    render_kernel.setArg(0, image);

    context.get_queue().enqueueNDRangeKernel(
            render_kernel,
            cl::NDRange(0, 0),
            cl::NDRange(width, height)
    );

    context.get_queue().enqueueReleaseGLObjects(&gl_objects);
    context.get_queue().finish();
}
