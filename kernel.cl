#define COMPONENT_FOLD(x) ( (x>1) ? (2-x) : ((x<-1) ?(-2-x):x))

float fPow = 8;
float shiftValue = 0.95;
float epsilon = 0.0001;
int itLimit = 128;
float r_min = 0.5f;
float escape_time = 100.0f;
float scale = 2.39128f;

/**********************************************************************************************************************/

typedef struct Vector {
    float x, y, z;
} Vector;

typedef struct Ray {
    Vector pos, dir;
} Ray;

typedef struct Pair {
    float x, y;
} Pair;

typedef struct Color {
    unsigned char r, g, b;
} Color;

typedef struct Hit {
    float distance;
    int depth;
} Hit;

typedef struct Camera {

    Vector pos, dir, up, u, w, v;

    float view_plane_distance, ratio, shift_multiplier;
    int width, height;

} Camera;

/**********************************************************************************************************************/

//float len(Vector * vector) {
//    return sqrtf(vector->x * vector->x + vector->y * vector->y + vector->z * vector->z);
//}
//
//void normalize(Vector * vector) {
//    float l = len(vector);
//
//    if (l == 0.0f) {
//        vector->x = 0.0;
//        vector->y = 0.0;
//        vector->z = 0.0;
//    }
//
//    else {
//        vector->x = vector->x / l;
//        vector->y = vector->y / l;
//        vector->z = vector->z / l;
//    }
//}
//
//void add(Vector * a, Vector * b, Vector * result) {
//    result->x = a->x + b->x;
//    result->y = a->y + b->y;
//    result->z = a->z + b->z;
//});
//    render.setArg(1, pixel_colors)
//
//void sub(Vector * a, Vector * b, Vector * result) {
//    result->x = a->x - b->x;
//    result->y = a->y - b->y;
//    result->z = a->z - b->z;
//}
//
//void cross(Vector * v1, Vector * v2, Vector * result) {
//    float a = v2->x, b = v2->y, c = v2->z;
//    float x = v1->x, y = v1->y, z = v1->z;
//
//    result->x = y * c - z * b;
//    result->y = x * c - a * z;
//    result->z = x * b - y * a;
//}
//
//float dot(Vector * v1, Vector * v2) {
//    return v1->x * v2->x + v1->y * v2->y + v1->z * v2->z;
//}
//
//void scalar_mul(Vector *v, float s, Vector *result) {
//    result->x = v->x * s;
//    result->y = v->y * s;
//    result->z = v->z * s;
//}
//
///**********************************************************************************************************************/
//
//void pow_vec(Vector *v, Vector *result) {
//    float ph = atanf(v->y / v->x);
//    float th = acosf(v->z / len(v));
//
//    result->x = sinf(fPow * th) * cosf(fPow * ph);
//    result->y = sinf(fPow * th) * sinf(fPow * ph);
//    result->z = cosf(fPow * th);
//
//    scalar_mul(result, powf(len(v), fPow), result);
//}
//
//float square(float x) { return x*x; }
//
//void fold_box(Vector *v) {
//
//    v->x = COMPONENT_FOLD(v->x);
//    v->y = COMPONENT_FOLD(v->y);
//    v->z = COMPONENT_FOLD(v->z);
//
//}
//
//void fold_sphere(Vector *v, float r2, float r_min_2, float r_fixed_2)
//{
//    if (r2 < r_min_2)
//        scalar_mul(v, r_fixed_2 / r_min_2, v);
//    else
//    if (r2 < r_fixed_2)
//        scalar_mul(v, r_fixed_2 / r2, v);
//}
//
//float distance(Vector * p0) {
//    Vector p = *p0;
//
//    float r_min_2 = square(r_min);
//    float r_fixed_2 = 1.0f;
//    float escape = square(escape_time);
//    float d_factor = 1;
//    float r2 = -1;
//
//    float c1 = fabsf(scale - 1.0f);
//    float c2 = powf(fabsf(scale), 1 - itLimit);
//
//    for (int i = 0; i < itLimit; i++) {
//        fold_box(&p);
//        r2 = dot(&p, &p);
//
//        fold_sphere(&p, r2, r_min_2, r_fixed_2);
//
//        scalar_mul(&p, scale, &p);
//        add(&p, p0, &p);
//
//        if (r2 < r_min_2)
//            d_factor *= (r_fixed_2 / r_min_2);
//        else if (r2<r_fixed_2)
//            d_factor *= (r_fixed_2 / r2);
//
//        d_factor = d_factor * fabsf(scale) + 1.0f;
//
//        if ( r2 > escape )
//            break;
//    }
//
//    r2 = len(&p);
//
//    return (r2 - c1) / d_factor - c2;
//}
//
//Hit march_ray(Ray *ray, float pathLen) {
//    Hit hit = { .distance = INFINITY };
//    Vector temp;
//
//    for (int i = 0; i < itLimit; i++) {
//        float d = distance(&ray->pos);
//
//        if (d < epsilon && !(isinf(d) || isnan(d))) {
//            hit.distance = pathLen;
//            hit.depth = i;
//
//            break;
//
//        } else {
//
//            scalar_mul(&ray->dir, d * shiftValue, &temp);
//            add(&temp,&ray->pos, &ray->pos);
//
//            pathLen += d * shiftValue;
//
//        }
//    }
//
//    return hit;
//}


__kernel void render(__global Camera * camera, __global char3 * pixels) {

}
