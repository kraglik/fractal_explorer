#ifndef FRACTAL_EXPLORER_QUATERNION_H
#define FRACTAL_EXPLORER_QUATERNION_H

#ifdef __APPLE__
#include <OpenCL/cl.hpp>
#else
#include <CL/cl.hpp>
#endif

class Quaternion {

public:
    Quaternion(float x, float y, float z, float w);
    Quaternion(cl_float3 v, float w);
    explicit Quaternion(cl_float4 v);
    explicit Quaternion(Quaternion other);

private:
    cl_float4 q;
};

#endif