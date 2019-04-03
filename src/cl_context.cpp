#include "cl_context.h"
#include <iostream>
#include <X11/X.h>
#include <X11/Xlib.h>
#include <GL/gl.h>
#include <GL/glx.h>
#include <GL/glu.h>

CLContext::CLContext(SDL_GLContext gl_context) {
    std::vector<cl::Platform> platforms;
    cl::Platform::get(&platforms);
    platform = platforms[0];

    std::vector<cl::Device> all_devices;
    platform.getDevices(CL_DEVICE_TYPE_GPU, &all_devices);

    device = all_devices[0];

    cl_context_properties properties[] = {
            CL_GL_CONTEXT_KHR, (cl_context_properties) glXGetCurrentContext(),
            CL_GLX_DISPLAY_KHR, (cl_context_properties) glXGetCurrentDisplay(),
            CL_CONTEXT_PLATFORM, (cl_context_properties) (platform)(),
            0
    };

    context = cl::Context({device}, properties);
    queue = cl::CommandQueue(context);
}
