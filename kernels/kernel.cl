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

float3 reflect(float3 direction, float3 normal) {
    return normalize(direction - 2 * dot(direction, normal) * normal);
}

#define COMPONENT_FOLD(x) ( (x>1) ? (2-x) : ((x<-1) ?(-2-x):x))

inline float square(float x) { return x*x; }

inline void fold_box(float3 *v) {
    v->x = COMPONENT_FOLD(v->x);
    v->y = COMPONENT_FOLD(v->y);
    v->z = COMPONENT_FOLD(v->z);
}

inline void fold_sphere(float3 *v, float r2, float r_min_2, float r_fixed_2)
{
    if (r2 < r_min_2)
        *v *= r_fixed_2 / r_min_2;
    else
    if (r2 < r_fixed_2)
        *v *= r_fixed_2 / r2;
}

float fractal_distance(float3 point) {

    float3 p = point;

    float r_min_2 = square(0.5f);
    float r_fixed_2 = 1.0f;
    float escape = square(100.0f);
    float d_factor = 1;
    float r2 = -1;
    float scale = 2.39128f;
    float c1 = fabs(scale - 1.0f);
    float c2 = native_powr(fabs(scale), 1 - 32);

     for (int i = 0; i < 16; i++) {
         fold_box(&p);
         r2 = dot(p, p);
         fold_sphere(&p, r2, r_min_2, r_fixed_2);
         p *= scale;
         p += point;
         if (r2 < r_min_2)
             d_factor *= (r_fixed_2 / r_min_2);
         else if (r2<r_fixed_2)
             d_factor *= (r_fixed_2 / r2);
         d_factor = d_factor * fabs(scale) + 1.0;
         if ( r2 > escape )
             break;
     }
    r2 = native_sqrt(dot(p, p));
    return (r2 - c1) / d_factor - c2;
}


float3 orbit_trap(float3 point) {
        
    float3 color = {1e20f, 1e20f, 1e20f};
    float3 new_color;
    float3 orbit = {0, 0, 0};
    float3 m = {1.0f, 1.0f, 1.0f};
    
    float3 p = point;

    float r_min_2 = square(0.5f);
    float r_fixed_2 = 1.0f;
    float escape = square(100.0f);
    float d_factor = 1;
    float r2 = -1;
    float scale = 2.39128f;

    float c1 = fabs(scale - 1.0f);
    float c2 = native_powr(fabs(scale), 1 - 16);

     for (int i = 0; i < 8; i++) {
         fold_box(&p);
         r2 = dot(p, p);

         fold_sphere(&p, r2, r_min_2, r_fixed_2);

         p *= scale;
         p += point;

         if (r2 < r_min_2)
             d_factor *= (r_fixed_2 / r_min_2);
         else if (r2<r_fixed_2)
             d_factor *= (r_fixed_2 / r2);

         d_factor = d_factor * fabs(scale) + 1.0;

         orbit = max(orbit, p * m);

         if ( r2 > escape )
             break;
     }

    r2 = native_sqrt(dot(p, p));

    return orbit;

}


float3 normal_to_fractal(float3 point) {

    float3 a = {epsilon, 0.0f, 0.0f};
    float3 b = {0.0f, epsilon, 0.0f};
    float3 c = {0.0f, 0.0f, epsilon};

    float3 result = {
        fractal_distance(point + a) - fractal_distance(point - a),
        fractal_distance(point + b) - fractal_distance(point - b),
        fractal_distance(point + c) - fractal_distance(point - c)
    };

    return normalize(result);
}


Hit march_ray(float3 position, float3 direction) {
    Hit hit = {.distance = 0.0f, .met = false};
    
    for (int i = 0; i < 250; i++) {
    
         float d = fractal_distance(position);
         position += d * direction;
         hit.distance += d;

         if (d < epsilon) {
             hit.position = position;
             hit.normal = normal_to_fractal(position);
             hit.met = true;
             break;
         }

         if (d > 100.0f) {
             hit.distance = 1e20f;
             break;
         }
    }
        
    return hit;
}


float3 blinn_phong(float3 position,
                   float3 normal,
                   float3 direction,
                   float shadow_coefficient) {

    float3 color_diffusive = normalize(orbit_trap(position));
    float3 sun_direction = {-1, 1, -1};
    sun_direction = normalize(sun_direction);

    float3 color_specular = color_diffusive;

    float3 color;

    float diffusive = 0.0f;
    float specular = 0.0f;

    position += normal * epsilon * 2;

    float projection_length = dot(normal, sun_direction);

    if (projection_length > 0.0f) {

        diffusive = max(0.0f, projection_length);
        specular = max(0.0f, pow(dot(reflect(-sun_direction, normal), direction), 3.0f));

        if (march_ray(position, sun_direction).met) {
            diffusive *= shadow_coefficient;
            specular *= shadow_coefficient;
        }

    } else {

        diffusive = 0.15f;
        specular = 0.0f;

    }
     
    color = clamp(diffusive * color_diffusive + specular * color_specular, 0.0f, 1.0f);

    return color;
}


float4 render_color(float x, float y, __constant Camera * camera) {
    float3 forward = camera->direction;
    float3 up = camera->up;
    float3 right = camera->right;
    float3 position = camera->position;

    float3 direction = normalize(right * x + up * y + forward);
    
    Hit hit = march_ray(position, direction);
    
    float4 color = {0, 0, 0, 1};
    
     if (hit.distance < 100.0f) {

         float3 blinn_phong_color = blinn_phong(hit.position, hit.normal, direction, 0.6f) * 0.75f;

         color.x = blinn_phong_color.x;
         color.y = blinn_phong_color.y;
         color.z = blinn_phong_color.z;
         
         float3 reflected_direction = reflect(direction, hit.normal);

         Hit reflected_hit = march_ray(hit.position + hit.normal * 2 * epsilon, reflected_direction);

         if (reflected_hit.met) {

             blinn_phong_color = blinn_phong(reflected_hit.position, reflected_hit.normal, reflected_direction, 0.5f) * 0.25f;

             color.x += blinn_phong_color.x;
             color.y += blinn_phong_color.y;
             color.z += blinn_phong_color.z;

         }
         else {

             color.x += 0.25f * 135 / 255.0f;
             color.y += 0.25f * 206 / 255.0f;
             color.z += 0.25f * 235.0f / 255.0f;

         }

     }

     else {
         color.x = 135 / 255.0f;
         color.y = 206 / 255.0f;
         color.z = 235.0f / 255.0f;
     }
    
    return color;    
}

__kernel void render(__write_only image2d_t result, __constant Camera * camera) {
    int2 pixel_position = { get_global_id(0), get_global_id(1) };
    
    float size_x = get_global_size(0);
    float size_y = get_global_size(1);
    float ratio = size_x / size_y;
    
    float half_size_x = size_x / 2;
    float half_size_y = size_y / 2;
    
    float x = (pixel_position.x - half_size_x) / half_size_x * ratio;
    float y = (pixel_position.y - half_size_y) / half_size_y;
    
    write_imagef(result, pixel_position, render_color(x, y, camera));
}
