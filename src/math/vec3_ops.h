#ifndef FRACTAL_EXPLORER_VEC3_OPS_H
#define FRACTAL_EXPLORER_VEC3_OPS_H

#ifdef __APPLE__
#include <OpenCL/cl.hpp>
#else
#include <CL/cl.hpp>
#endif

cl_float3 operator^(cl_float3 a, cl_float3 b);

cl_float3 operator+(cl_float3 a, cl_float3 b);
cl_float3 operator-(cl_float3 a, cl_float3 b);
cl_float3 operator*(cl_float3 a, cl_float3 b);
cl_float3 operator/(cl_float3 a, cl_float3 b);

cl_float3 operator+(cl_float3 vec, float m);
cl_float3 operator-(cl_float3 vec, float m);
cl_float3 operator*(cl_float3 vec, float m);
cl_float3 operator/(cl_float3 vec, float m);

cl_float3 normalize(cl_float3 v);
cl_float3 abs(cl_float3 v);
cl_float3 clamp(cl_float3 v, float min, float max);
cl_float3 clamp(cl_float3 v, cl_float3 min, cl_float3 max);
float dot(cl_float3 a, cl_float3 b);
float len(cl_float3 v);

#endif