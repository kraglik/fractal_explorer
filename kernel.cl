#define COMPONENT_FOLD(x) ( (x>1) ? (2-x) : ((x<-1) ?(-2-x):x))

typedef struct WorldProps {
    float shiftValue;
    float epsilon;
    int itLimit;
    float r_min;
    float escape_time;
    float scale;
} WorldProps;

typedef struct Ray {
    float3 pos, dir;
} Ray;

typedef struct Hit {
    float distance;
    int depth;
} Hit;

typedef struct Camera {

    float3 pos, dir, up, right;

    float view_plane_distance, ratio, shift_multiplier;
    int width, height;

} Camera;

/**********************************************************************************************************************/

float len(float3 * vector) {
    return sqrt(vector->x * vector->x + vector->y * vector->y + vector->z * vector->z);
}

/**********************************************************************************************************************/

float square(float x) { return x*x; }

void fold_box(float3 *v) {

    v->x = COMPONENT_FOLD(v->x);
    v->y = COMPONENT_FOLD(v->y);
    v->z = COMPONENT_FOLD(v->z);

}

void fold_sphere(float3 *v, float r2, float r_min_2, float r_fixed_2)
{
    if (r2 < r_min_2)
        *v *= r_fixed_2 / r_min_2;
    else
    if (r2 < r_fixed_2)
        *v *= r_fixed_2 / r2;
}

float mandelbox_distance(float3 *p0, __global WorldProps * props) {
    float3 p = *p0;

    float r_min_2 = square(props->r_min);
    float r_fixed_2 = 1.0f;
    float escape = square(props->escape_time);
    float d_factor = 1;
    float r2 = -1;
    float scale = props->scale;

    float c1 = fabs(scale - 1.0f);
    float c2 = pow(fabs(scale), 1 - props->itLimit);

    for (int i = 0; i < props->itLimit; i++) {
        fold_box(&p);
        r2 = dot(p, p);

        fold_sphere(&p, r2, r_min_2, r_fixed_2);

        p *= scale;
        p += *p0;

        if (r2 < r_min_2)
            d_factor *= (r_fixed_2 / r_min_2);
        else if (r2<r_fixed_2)
            d_factor *= (r_fixed_2 / r2);

        d_factor = d_factor * fabs(scale) + 1.0;

        if ( r2 > escape )
            break;
    }

    r2 = sqrt(dot(p, p));

    return (r2 - c1) / d_factor - c2;
}

Hit march_ray(Ray *ray, __global WorldProps * props, float pathLen) {
    Hit hit = { .distance = INFINITY };
    float3 temp;

    float epsilon = props->epsilon;

    for (int i = 0; i < props->itLimit; i++) {
        float d = mandelbox_distance(&ray->pos, props);

        if (d < epsilon && !(isinf(d) || isnan(d))) {
            hit.distance = pathLen;
            hit.depth = i;

            break;

        } else {

            ray->pos += ray->dir * d * props->shiftValue;
            pathLen += d * props->shiftValue;

        }
    }

    return hit;
}

void move_ray_to_view_plane(Ray * ray, __global Camera * camera, __global WorldProps * props) {
    float shift_value = 1.0f / dot(ray->dir, camera->dir);
    ray->pos += ray->dir * shift_value * camera->shift_multiplier;
}


Hit trace_ray(__global Camera *camera, __global WorldProps * props, float x, float y) {
    float m = camera->shift_multiplier;

    Ray ray = {.pos = camera->pos + camera-> right * x * m + camera->up * y * m};

    ray.dir = normalize(camera->dir * camera->view_plane_distance + ray.pos - camera->pos);

    return march_ray(&ray, props, 0.0f);
}

__kernel void render(__global Camera * camera, __global WorldProps * props, __global uchar3 * pixels) {
    int idX = get_global_id(0);
    int idY = get_global_id(1);

    int pixel_id = idY * camera->width + idX;

    float hx = (float)camera->width / 2.0f;
    float hy = (float)camera->height / 2.0f;

    float x = ((float)idX - hx) / hx * camera->ratio;
    float y = ((float)idY - hy) / hy;

    Hit hit = trace_ray(camera, props, x, y);

    float color_strength = 1.0f - (float)hit.depth / (float)props->itLimit;

    if (!isinf(hit.distance)) {

        pixels[pixel_id].x = (unsigned char)clamp((color_strength * 229.0f), 0.0f, 229.0f);
        pixels[pixel_id].y = (unsigned char)clamp((color_strength * 210.0f), 0.0f, 210.0f);
        pixels[pixel_id].z = (unsigned char)clamp((color_strength * 180.0f), 0.0f, 180.0f);

    } else {

        pixels[pixel_id].x = 0;
        pixels[pixel_id].y = 0;
        pixels[pixel_id].z = 0;

    }
}
