#ifndef FRACTAL_EXPLORER_VEC3_OPS_H
#define FRACTAL_EXPLORER_VEC3_OPS_H

#ifdef __APPLE__
#include <OpenCL/cl.hpp>
#else
#include <CL/cl.hpp>
#endif

cl_float3 operator^(const cl_float3 & a, const cl_float3 & b);

cl_float3 operator+(const cl_float3 & a, const cl_float3 & b);
cl_float3 operator-(const cl_float3 & a, const cl_float3 & b);
cl_float3 operator*(const cl_float3 & a, const cl_float3 & b);
cl_float3 operator/(const cl_float3 & a, const cl_float3 & b);

cl_float3 operator+(const cl_float3 & vec, const cl_float m);
cl_float3 operator-(const cl_float3 & vec, const cl_float m);
cl_float3 operator*(const cl_float3 & vec, const cl_float m);
cl_float3 operator/(const cl_float3 & vec, const cl_float m);

cl_float3 & operator+=(cl_float3 & a, const cl_float3 & b);
cl_float3 & operator-=(cl_float3 & a, const cl_float3 & b);
cl_float3 & operator*=(cl_float3 & a, const cl_float3 & b);
cl_float3 & operator/=(cl_float3 & a, const cl_float3 & b);

cl_float3 & operator+=(cl_float3 & vec, const cl_float m);
cl_float3 & operator-=(cl_float3 & vec, const cl_float m);
cl_float3 & operator*=(cl_float3 & vec, const cl_float m);
cl_float3 & operator/=(cl_float3 & vec, const cl_float m);


cl_float3 normalize(const cl_float3 & v);
cl_float3 abs(const cl_float3 & v);
cl_float3 clamp(const cl_float3 & v, cl_float min, cl_float max);
cl_float3 clamp(const cl_float3 & v, const cl_float3 & min, const cl_float3 & max);
cl_float dot(const cl_float3 & a, const cl_float3 & b);
cl_float len(const cl_float3 & v);

#endif