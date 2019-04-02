#include "vec3_ops.h"
#include <cmath>

cl_float3 operator^(cl_float3 a, cl_float3 b) {
    return {
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x
    };
}

cl_float3 operator+(cl_float3 a, cl_float3 b) {
    return {
        a.x + b.x,
        a.y + b.y,
        a.z + b.z
    };
}

cl_float3 operator-(cl_float3 a, cl_float3 b) {
    return {
            a.x - b.x,
            a.y - b.y,
            a.z - b.z
    };
}

cl_float3 operator*(cl_float3 a, cl_float3 b) {
    return {
            a.x * b.x,
            a.y * b.y,
            a.z * b.z
    };
}

cl_float3 operator/(cl_float3 a, cl_float3 b) {
    return {
            a.x / b.x,
            a.y / b.y,
            a.z / b.z
    };
}

cl_float3 operator+(cl_float3 vec, float m) {
    return {
            vec.x + m,
            vec.y + m,
            vec.z + m
    };
}

cl_float3 operator-(cl_float3 vec, float m) {
    return {
            vec.x - m,
            vec.y - m,
            vec.z - m
    };
}

cl_float3 operator*(cl_float3 vec, float m) {
    return {
            vec.x * m,
            vec.y * m,
            vec.z * m
    };
}

cl_float3 operator/(cl_float3 vec, float m) {
    return {
            vec.x / m,
            vec.y / m,
            vec.z / m
    };
}

cl_float3 normalize(cl_float3 v) {
    float l = len(v);

    if (l == 0)
        return {0, 0, 0};

    return v / len(v);
}

float len(cl_float3 v) {
    return std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

float dot(cl_float3 a, cl_float3 b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

cl_float3 abs(cl_float3 v) {
    return {
        std::abs(v.x),
        std::abs(v.y),
        std::abs(v.z)
    };
}

cl_float3 clamp(cl_float3 v, float min, float max) {
    return {
        std::min(std::max(min, v.x), max),
        std::min(std::max(min, v.y), max),
        std::min(std::max(min, v.z), max)
    };
}

cl_float3 clamp(cl_float3 v, cl_float3 min, cl_float3 max) {
    return {
        std::min(std::max(min.x, v.x), max.x),
        std::min(std::max(min.y, v.y), max.y),
        std::min(std::max(min.z, v.z), max.z)
    };
}