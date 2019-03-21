#include <iostream>
#include <fstream>
#include <math.h>
#include "bitmap/bitmap_image.hpp"

#define __CL_ENABLE_EXCEPTIONS

#ifdef __APPLE__
#include <OpenCL/cl.hpp>
#else
#include <CL/cl.hpp>
#endif


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

float len(Vector * vector) {
    return std::sqrt(vector->x * vector->x + vector->y * vector->y + vector->z * vector->z);
}

void normalize(Vector * vector) {
    float l = len(vector);

    if (l == 0.0f) {
        vector->x = 0.0;
        vector->y = 0.0;
        vector->z = 0.0;
    }

    else {
        vector->x = vector->x / l;
        vector->y = vector->y / l;
        vector->z = vector->z / l;
    }
}

void add(Vector * a, Vector * b, Vector * result) {
    result->x = a->x + b->x;
    result->y = a->y + b->y;
    result->z = a->z + b->z;
}

void sub(Vector * a, Vector * b, Vector * result) {
    result->x = a->x - b->x;
    result->y = a->y - b->y;
    result->z = a->z - b->z;
}

void cross(Vector * v1, Vector * v2, Vector * result) {
    float a = v2->x, b = v2->y, c = v2->z;
    float x = v1->x, y = v1->y, z = v1->z;

    result->x = y * c - z * b;
    result->y = x * c - a * z;
    result->z = x * b - y * a;
}

float dot(Vector * v1, Vector * v2) {
    return v1->x * v2->x + v1->y * v2->y + v1->z * v2->z;
}

void scalar_mul(Vector *v, float s, Vector *result) {
    result->x = v->x * s;
    result->y = v->y * s;
    result->z = v->z * s;
}


Camera build_camera(
        int width,
        int height,
        float shift_multiplier,
        float view_plane_distance,
        float ratio,
        Vector position,
        Vector target,
        Vector up) {

    Camera camera = {
            .pos = position,
            .up = up,
            .view_plane_distance = view_plane_distance,
            .ratio = ratio,
            .shift_multiplier = shift_multiplier,
            .width = width,
            .height = height
    };

    sub(&target, &position, &camera.dir);
    normalize(&camera.dir);

    sub(&position, &target, &camera.w);
    normalize(&camera.w);

    cross(&up, &camera.w, &camera.u);
    normalize(&camera.u);

    cross(&camera.w, &camera.u, &camera.v);
    normalize(&camera.v);

    return camera;
}


int main(int, char**) {
    int width = 500, height = 500;
    int m_width = width / 2, m_height = height / 2;

    float brightness = 1.25f;

    bitmap_image image(width, height);

    Vector camera_position = {.x = 7.0f, .y = 1.0f, .z = 7.0f};

    Vector camera_target = {.x = 0.0f, .y = -0.5f, .z = 0.0f};
    Vector camera_up = {.x = 0.0f, .y = 1.0f, .z = 0.0f};

    float view_plane_distance = 1.0f;
    float shift_multiplier = view_plane_distance >= 1.0f ? 0.25f / view_plane_distance : view_plane_distance / 4.0f;
    float ratio = 1.0f;

    Camera camera = build_camera(
            width,
            height,
            shift_multiplier,
            view_plane_distance,
            ratio,
            camera_position,
            camera_target,
            camera_up
    );

    std::vector<cl::Platform> platforms;
    cl::Platform::get(&platforms);
    cl::Platform platform = platforms[0];

    std::vector<cl::Device> all_devices;
    platform.getDevices(CL_DEVICE_TYPE_ALL, &all_devices);

    cl::Device device = all_devices[0];

    cl::Context context({device});
    cl::Program::Sources sources;

    std::ifstream kernel_file("kernel.cl");

    if (!kernel_file.is_open()) {
        return 2;
    }

    std::string * kernel_source = new std::string(
                        std::istreambuf_iterator<char>(kernel_file),
                        std::istreambuf_iterator<char>()
                    );

    sources.push_back({kernel_source->c_str(), kernel_source->length()});

    cl::CommandQueue queue(context, device);
    cl::Program program(context, sources);

    if (program.build({device}) != CL_SUCCESS) {
        std::cout << "Error building: " << program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(device) << std::endl;
        exit(1);
    }

    cl::Kernel render = cl::Kernel(program, "render");

    cl_char3 colors[width * height];

    cl::Buffer pixel_colors(context, CL_MEM_READ_WRITE, sizeof(cl_char3) * width * height);
    cl::Buffer camera_buffer(context, CL_MEM_READ_WRITE, sizeof(Camera));

    queue.enqueueWriteBuffer(camera_buffer, CL_TRUE, 0, sizeof(Camera), &camera);

    render.setArg(0, camera_buffer);
    render.setArg(1, pixel_colors);

    queue.enqueueNDRangeKernel(
            render,
            cl::NullRange,  // kernel, offset
            cl::NDRange(width), // global number of work items
            cl::NDRange(height)
    );

    queue.enqueueReadBuffer(pixel_colors, CL_TRUE, 0, sizeof(cl_char3) * width * height, colors);
    queue.finish();

    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            cl_char3 pixel_color = colors[i * height + j];
            image.set_pixel(i, j, pixel_color.x, pixel_color.y, pixel_color.z);
        }
    }

    image.save_image("result.bmp");
}