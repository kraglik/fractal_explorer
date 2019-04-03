#include "vec3_ops.h"
#include <cmath>

cl_float3 operator^(const cl_float3 & a, const cl_float3 & b) {
    return {
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x
    };
}

cl_float3 operator+(const cl_float3 & a, const cl_float3 & b) {
    return {
        a.x + b.x,
        a.y + b.y,
        a.z + b.z
    };
}

cl_float3 operator-(const cl_float3 & a, const cl_float3 & b) {
    return {
            a.x - b.x,
            a.y - b.y,
            a.z - b.z
    };
}

cl_float3 operator*(const cl_float3 & a, const cl_float3 & b) {
    return {
            a.x * b.x,
            a.y * b.y,
            a.z * b.z
    };
}

cl_float3 operator/(const cl_float3 & a, const cl_float3 & b) {
    return {
            a.x / b.x,
            a.y / b.y,
            a.z / b.z
    };
}

cl_float3 operator+(const cl_float3 & vec, const cl_float m) {
    return {
            vec.x + m,
            vec.y + m,
            vec.z + m
    };
}

cl_float3 operator-(const cl_float3 & vec, const cl_float m) {
    return {
            vec.x - m,
            vec.y - m,
            vec.z - m
    };
}

cl_float3 operator*(const cl_float3 & vec, const cl_float m) {
    return {
            vec.x * m,
            vec.y * m,
            vec.z * m
    };
}

cl_float3 operator/(const cl_float3 & vec, const cl_float m) {
    return {
            vec.x / m,
            vec.y / m,
            vec.z / m
    };
}

cl_float3 normalize(const cl_float3 & v) {
    float l = len(v);

    if (l == 0)
        return {0, 0, 0};

    return v / len(v);
}

cl_float len(const cl_float3 & v) {
    return std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

cl_float dot(const cl_float3 & a, const cl_float3 & b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

cl_float3 abs(const cl_float3 & v) {
    return {
        std::abs(v.x),
        std::abs(v.y),
        std::abs(v.z)
    };
}

cl_float3 clamp(const cl_float3 & v, const cl_float min, const cl_float max) {
    return {
        std::min(std::max(min, v.x), max),
        std::min(std::max(min, v.y), max),
        std::min(std::max(min, v.z), max)
    };
}

cl_float3 clamp(const cl_float3 & v, const cl_float3 & min, const cl_float3 & max) {
    return {
        std::min(std::max(min.x, v.x), max.x),
        std::min(std::max(min.y, v.y), max.y),
        std::min(std::max(min.z, v.z), max.z)
    };
}

cl_float3 & operator+=(cl_float3 & a, const cl_float3 & b) {

    a.x += b.x;
    a.y += b.y;
    a.z += b.z;

    return a;
}

cl_float3 & operator-=(cl_float3 & a, const cl_float3 & b) {

    a.x -= b.x;
    a.y -= b.y;
    a.z -= b.z;

    return a;
}

cl_float3 & operator*=(cl_float3 & a, const cl_float3 & b) {

    a.x *= b.x;
    a.y *= b.y;
    a.z *= b.z;

    return a;
}

cl_float3 & operator/=(cl_float3 & a, const cl_float3 & b) {

    a.x /= b.x;
    a.y /= b.y;
    a.z /= b.z;

    return a;
}

cl_float3 & operator+=(cl_float3 & vec, const cl_float m) {

    vec.x += m;
    vec.y += m;
    vec.z += m;

    return vec;
}

cl_float3 & operator-=(cl_float3 & vec, const cl_float m) {

    vec.x -= m;
    vec.y -= m;
    vec.z -= m;

    return vec;
}

cl_float3 & operator*=(cl_float3 & vec, const cl_float m) {

    vec.x *= m;
    vec.y *= m;
    vec.z *= m;

    return vec;
}

cl_float3 & operator/=(cl_float3 & vec, const cl_float m) {

    vec.x /= m;
    vec.y /= m;
    vec.z /= m;

    return vec;
}