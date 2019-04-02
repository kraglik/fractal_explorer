#include <iostream>
#include <fstream>
#include <cmath>

//#define __CL_ENABLE_EXCEPTIONS

#ifdef __APPLE__
#include <OpenCL/cl.hpp>
#else
#include <CL/cl.hpp>
#endif

#include "src/math/vec3_ops.h"


int main(int, char**) {
    cl_float3 a = {1, 0, 0}, b = {0, 1, 0};
    auto c = a ^ b;

    std::cout << c.x << ", " << c.y << ", " << c.z << std::endl;

    return 0;
}