#ifndef FRACTAL_EXPLORER_CAMERA_H
#define FRACTAL_EXPLORER_CAMERA_H

#ifdef __APPLE__
#include <OpenCL/cl.hpp>
#else
#include <CL/cl.hpp>
#endif

class Camera {

    public:
        typedef struct {
            cl_float3 position, direction, right, world_up, up;
            cl_float zoom, focus;
        } CameraCL;

        Camera(cl_float3 pos, cl_float3 dir, cl_float3 up, cl_float zoom, cl_float focus);

        cl_float3 get_position() const;
        cl_float3 get_direction() const;
        cl_float3 get_up() const;
        cl_float3 get_world_up() const;
        cl_float3 get_right() const;
        cl_float get_zoom() const;
        cl_float get_focus() const;
        CameraCL get_camera_cl() const;

        void set_zoom(cl_float new_zoom);
        void set_focus(cl_float new_focus);
        void look_at(cl_float3 target);

    private:
        cl_float3 position, direction, right, world_up, up;
        cl_float zoom, focus;

};


#endif