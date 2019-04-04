#include "camera.h"
#include "math/vec3_ops.h"
#include "math/quaternion.h"
#include <cmath>


Camera::Camera(cl_float3 pos,
               cl_float3 dir,
               cl_float3 up,
               cl_float zoom,
               cl_float focus,
               cl_float movement_speed,
               cl_float rotation_speed) {

    position = pos;
    world_up = normalize(up);

    this->movement_speed = movement_speed;
    this->rotation_speed = rotation_speed;
    this->zoom = zoom;
    this->focus = focus;

    look_at(position + normalize(dir));
}


Camera::CameraCL Camera::get_camera_cl() const {
    return {
      .position = position,
      .direction = direction,
      .right = right,
      .world_up = world_up,
      .up = up,
      .zoom = zoom,
      .focus = focus
    };
}

void Camera::look_at(cl_float3 target) {
    direction = normalize(target - position);
    right = normalize(direction ^ world_up);
    up = normalize(right ^ direction);
}

void Camera::move(cl_float3 dir, cl_float delta_time) {
    position += normalize(dir) * movement_speed * delta_time;
}


void Camera::pitch(cl_float angle) {
    auto q_1 = Quaternion::from_axis_angle(right, angle / zoom);
    Quaternion q_2(direction, 0.0f);

    look_at(position + direction + (q_1 * q_2).xyz());
}

void Camera::yaw(cl_float angle) {
    auto q_1 = Quaternion::from_axis_angle(world_up, angle / zoom);
    Quaternion q_2(direction, 0.0f);

    look_at(position + direction + (q_1 * q_2).xyz());
}

void Camera::rotate(cl_float dx, cl_float dy) {
    static const cl_float deg_to_rad = CL_M_PI / 180.0f;

    cl_float look_right_rads =  rotation_speed * dx * deg_to_rad;
    cl_float look_up_rads = rotation_speed * dy * deg_to_rad;

    cl_float current_dec = std::acos(direction.y);
    cl_float requested_dec = current_dec - look_up_rads;

    static cl_float min_up_tilt_deg = 1;
    static cl_float zenith_min_dec = min_up_tilt_deg * deg_to_rad;
    static cl_float zenith_max_dec = (180.0f - min_up_tilt_deg) * deg_to_rad;

    look_up_rads = requested_dec < zenith_min_dec ? zenith_min_dec : look_up_rads;
    look_up_rads = requested_dec > zenith_max_dec ? zenith_max_dec : look_up_rads;

    pitch(look_up_rads);
    yaw(-look_right_rads);
}
