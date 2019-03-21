#include <iostream>
#include <fstream>
#include <math.h>
#include "bitmap/bitmap_image.hpp"

//#define __CL_ENABLE_EXCEPTIONS

#ifdef __APPLE__
#include <OpenCL/cl.hpp>
#else
#include <CL/cl.hpp>
#endif

/**********************************************************************************************************************/

typedef struct Ray {
    cl_float3 pos, dir;
} Ray;

typedef struct Hit {
    cl_float distance;
    cl_int depth;
} Hit;

typedef struct Camera {

    cl_float3 pos, dir, up, right;

    cl_float view_plane_distance, ratio, shift_multiplier;
    cl_int width, height;

} Camera;

/**********************************************************************************************************************/

float len(cl_float3 * vector) {
    return std::sqrt(vector->x * vector->x + vector->y * vector->y + vector->z * vector->z);
}

void normalize(cl_float3 * vector) {
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

void add(cl_float3 * a, cl_float3 * b, cl_float3 * result) {
    result->x = a->x + b->x;
    result->y = a->y + b->y;
    result->z = a->z + b->z;
}

void sub(cl_float3 * a, cl_float3 * b, cl_float3 * result) {
    result->x = a->x - b->x;
    result->y = a->y - b->y;
    result->z = a->z - b->z;
}

void cross(cl_float3 a, cl_float3 b, cl_float3 * result) {

    result->x = a.y * b.z - a.z * b.y;
    result->y = a.z * b.x - a.x * b.z;
    result->z = a.x * b.y - a.y * b.x;
}

float dot(cl_float3 * v1, cl_float3 * v2) {
    return v1->x * v2->x + v1->y * v2->y + v1->z * v2->z;
}

void scalar_mul(cl_float3 *v, float s, cl_float3 *result) {
    result->x = v->x * s;
    result->y = v->y * s;
    result->z = v->z * s;
}


Camera build_camera(
        cl_int width,
        cl_int height,
        cl_float shift_multiplier,
        cl_float view_plane_distance,
        cl_float ratio,
        cl_float3 position,
        cl_float3 target,
        cl_float3 up) {

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

    cross(camera.dir, up, &camera.right);
    normalize(&camera.right);

    cross(camera.right, camera.dir, &camera.up);
    normalize(&camera.up);

    return camera;
}


int main(int, char**) {
    cl_int width = 750, height = 750;
    bitmap_image image(width, height);

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

    cl_uchar3 colors[width * height];

    cl::Buffer pixel_colors(context, CL_MEM_READ_WRITE, sizeof(cl_uchar3) * width * height);
    cl::Buffer camera_buffer(context, CL_MEM_READ_WRITE, sizeof(Camera));

    cl_int m_width = width / 2, m_height = height / 2;

    cl_float3 camera_target = {.x = 0.0f, .y = 0.0f, .z = 0.0f};
    cl_float3 camera_up = {.x = 0.0f, .y = 1.0f, .z = 0.0f};

    cl_float view_plane_distance = 1.0f;
    cl_float shift_multiplier = view_plane_distance >= 1.0f ? 0.25f / view_plane_distance : view_plane_distance / 4.0f;
    cl_float ratio = 1.0f;

    for (int k = 0; k < 360; k++) {

        cl_float3 camera_position = {
                .x = 10.0f * sin((float) k / (360.0f / 4.0f) * (float)M_PI),
                .y = 0.0f,
                .z = 10.0f * cos((float) k / (360.0f / 4.0f) * (float)M_PI)
        };

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

        queue.enqueueWriteBuffer(camera_buffer, CL_TRUE, 0, sizeof(Camera), &camera);

        render.setArg(0, camera_buffer);
        render.setArg(1, pixel_colors);

        queue.enqueueNDRangeKernel(
                render,
                cl::NDRange(0, 0),  // kernel, offset
                cl::NDRange(width, height) // global number of work items
        );

        queue.enqueueBarrierWithWaitList();

        queue.enqueueReadBuffer(pixel_colors, CL_TRUE, 0, sizeof(cl_uchar3) * width * height, colors);
        queue.finish();

        for (int i = 0; i < width; i++) {
            for (int j = 0; j < height; j++) {
                cl_uchar3 pixel_color = colors[i * height + j];
                image.set_pixel(j, i, pixel_color.x, pixel_color.y, pixel_color.z);
            }
        }

        image.save_image("renders/result_" + std::to_string(k) + ".bmp");
    }
}