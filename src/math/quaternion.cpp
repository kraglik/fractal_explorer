#include "quaternion.h"

Quaternion::Quaternion(Quaternion const & other) {
    q = other.q;
}

Quaternion::Quaternion(cl_float3 v, cl_float w) {
    q.x = v.x;
    q.y = v.y;
    q.z = v.z;
    q.w = w;
}

Quaternion::Quaternion(cl_float x, cl_float y, cl_float z, cl_float w) {
    q.x = x;
    q.y = y;
    q.z = z;
    q.w = w;
}

void Quaternion::normalize() {
    cl_float l = std::sqrt(q.x * q.x + q.y * q.y + q.z * q.z + q.w * q.w);

    q.x /= l;
    q.y /= l;
    q.z /= l;
    q.w /= l;
}

Quaternion& Quaternion::operator=(const Quaternion &other) {
    q = other.q;

    return *this;
}

Quaternion operator*(const Quaternion & a, const Quaternion & b) {
    cl_float3 l = a.xyz();
    cl_float3 r = b.xyz();

    auto xyz = l * b.w() + r * a.w() + l ^ r;

    return Quaternion(xyz, a.w() * b.w() - dot(l, r));
}