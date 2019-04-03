#ifndef FRACTAL_EXPLORER_QUATERNION_H
#define FRACTAL_EXPLORER_QUATERNION_H

#ifdef __APPLE__
#include <OpenCL/cl.hpp>
#else
#include <CL/cl.hpp>
#endif
#include <cmath>
#include "vec3_ops.h"

class Quaternion {

    public:
        Quaternion(cl_float x, cl_float y, cl_float z, cl_float w);
        Quaternion(cl_float3 v, cl_float w);
        Quaternion(Quaternion const & other);
        Quaternion() = default;

        static Quaternion from_axis_angle(cl_float3 axis, cl_float w) {
            cl_float sin_h_w = std::sin(w * 0.5f);

            axis *= sin_h_w;
            w = std::cos(w * 0.5f);

            Quaternion q(axis, w);
            q.normalize();

            return q;
        }

        void normalize();

        Quaternion& operator=(const Quaternion & other);

        cl_float x() const { return q.x; }
        cl_float y() const { return q.y; }
        cl_float z() const { return q.z; }
        cl_float w() const { return q.w; }
        cl_float2 xy() const { return {q.x, q.y}; }
        cl_float3 xyz() const { return {q.x, q.y, q.z}; }
        cl_float4 xyzw() const { return q; }

        void set_x(cl_float new_x) { q.x = new_x; }
        void set_y(cl_float new_y) { q.y = new_y; }
        void set_z(cl_float new_z) { q.z = new_z; }
        void set_w(cl_float new_w) { q.w = new_w; }

        friend Quaternion operator*(const Quaternion & a, const Quaternion & b);

    private:
        cl_float4 q = {0, 0, 0, 0};
};

Quaternion operator*(const Quaternion & a, const Quaternion & b);

#endif