typedef struct Camera {
    float3 position, direction, right, world_up, up;
    float zoom, focus;
} Camera;


__kernel void render(__write_only image2d_t result) {
    int2 pixel_position = { get_global_id(0), get_global_id(1) };
    float4 white_color = {1.0f, 1.0f, 1.0f, 1.0f};
    float4 black_color = {0.0f, 0.0f, 0.0f, 1.0f};

    if (pixel_position.x == pixel_position.y) {
        write_imagef(result, pixel_position, white_color);
    } else {
        write_imagef(result, pixel_position, black_color);
    }
}