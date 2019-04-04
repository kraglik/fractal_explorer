#define epsilon 0.00001f

typedef struct Camera {
    float3 position, direction, right, world_up, up;
    float zoom, focus;
} Camera;

typedef struct Hit {
    float3 position, normal;
    float distance;
    bool met;
} Hit;
