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

        Camera(
            cl_float3 pos, cl_float3 dir, cl_float3 up,
            cl_float zoom = 1.0f,
            cl_float focus = 0.0f,
            cl_float movement_speed = 0.01f,
            cl_float rotation_speed = 10.0f
        );

        cl_float3 get_position() const { return position; }
        cl_float3 get_direction() const { return direction; }
        cl_float3 get_up() const { return up; }
        cl_float3 get_world_up() const { return world_up; }
        cl_float3 get_right() const { return right; }
        cl_float get_zoom() const { return zoom; }
        cl_float get_focus() const { return focus; }
        cl_float get_movement_speed() const { return movement_speed; }
        cl_float get_rotation_speed() const { return rotation_speed; }
        CameraCL get_camera_cl() const;

        void set_zoom(cl_float new_zoom) { zoom = new_zoom; }
        void set_focus(cl_float new_focus) { focus = new_focus; }
        void set_movement_speed(cl_float new_speed) { movement_speed = new_speed; }
        void set_rotation_speed(cl_float new_speed) { rotation_speed = new_speed; }

        void look_at(cl_float3 target);
        void move(cl_float3 direction, cl_float delta_time);
        void rotate(cl_float dx, cl_float dy);

    private:
        cl_float3 position = {0, 0, 0};
        cl_float3 direction = {1, 0, 0};
        cl_float3 right = {0, 0, 1};
        cl_float3 world_up = {0, 1, 0};
        cl_float3 up = {0, 1, 0};
        cl_float zoom, focus, movement_speed, rotation_speed;

        void pitch(cl_float angle);
        void yaw(cl_float angle);

};


#endif