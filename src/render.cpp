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
        std::cout << program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(cl_context.get_device()) << std::endl;
        throw * new std::runtime_error("Building kernel failed.");
    }

    render_kernel = cl::Kernel(program, "render");

    camera_buffer = cl::Buffer(
        cl_context.get_context(),
        CL_MEM_READ_ONLY,
        sizeof(Camera::CameraCL)
    );
}

void Render::render(cl::Image2D &image, int width, int height) {

    std::vector<cl::Memory> gl_objects = {image};

    auto camera_host_buffer = camera.get_camera_cl();

    context.get_queue().enqueueWriteBuffer(camera_buffer, CL_TRUE, 0, sizeof(Camera::CameraCL), &camera_host_buffer);

    context.get_queue().enqueueAcquireGLObjects(&gl_objects);
    render_kernel.setArg(0, image);
    render_kernel.setArg(1, camera_buffer);

    context.get_queue().enqueueNDRangeKernel(
            render_kernel,
            cl::NDRange(0, 0),
            cl::NDRange(width, height)
    );

    context.get_queue().enqueueReleaseGLObjects(&gl_objects);
    context.get_queue().finish();
}
