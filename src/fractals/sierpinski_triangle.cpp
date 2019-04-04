#include "sierpinski_triangle.h"

const std::string distance_function_code =
        "float fractal_distance(float3 z) {\n"
        "    \n"
        "    float scale = 2.0f;\n"
        "    float offset = 1.5f;\n"
        "    \n"
        "    float temp_x, temp_y, temp_z;\n"
        "    \n"
        "    for (int n = 0; n < 24; n++) {\n"
        "    \n"
        "        if ((z.x + + z.y) < 0.0) {\n"
        "            temp_x = -z.y;\n"
        "            temp_y = -z.x;\n"
        "            z.x = temp_x;\n"
        "            z.y = temp_y;\n"
        "        }\n"
        "        \n"
        "        if ((z.x + z.z) < 0.0) {\n"
        "            temp_x = -z.z;\n"
        "            temp_z = -z.x;\n"
        "            z.x = temp_x;\n"
        "            z.z = temp_z;\n"
        "        }\n"
        "        \n"
        "        if ((z.z + z.y) < 0.0) {\n"
        "            temp_z = -z.y;\n"
        "            temp_y = -z.z;\n"
        "            z.z = temp_z;\n"
        "            z.y = temp_y;\n"
        "        }\n"
        "       \n"
        "        z = scale * z - offset * (scale - 1.0f);\n"
        "    }\n"
        " \n"
        "    return fast_length(z) * native_powr(scale, (float)(-24));\n"
        "}\n";

const std::string orbit_function_code =
        "float3 orbit_trap(float3 z) {\n"
        "\n"
        "   float3 color = {1e20f, 1e20f, 1e20f};\n"
        "   float3 new_color;\n"
        "   float3 orbit = {0, 0, 0};\n"
        "   float3 m = {0.42f, 0.38f, 0.19f};\n"
        "\n"
        "   float scale = 2.0f;\n"
        "   float offset = 1.5f;\n"
        "   \n"
        "   float temp_x, temp_y, temp_z;\n"
        "   \n"
        "   for (int n = 0; n < 24; n++) {\n"
        "   \n"
        "       if ((z.x + + z.y) < 0.0) {\n"
        "           temp_x = -z.y;\n"
        "           temp_y = -z.x;\n"
        "           z.x = temp_x;\n"
        "           z.y = temp_y;\n"
        "       }\n"
        "       \n"
        "       if ((z.x + z.z) < 0.0) {\n"
        "           temp_x = -z.z;\n"
        "           temp_z = -z.x;\n"
        "           z.x = temp_x;\n"
        "           z.z = temp_z;\n"
        "       }\n"
        "       \n"
        "       if ((z.z + z.y) < 0.0) {\n"
        "           temp_z = -z.y;\n"
        "           temp_y = -z.z;\n"
        "           z.z = temp_z;\n"
        "           z.y = temp_y;\n"
        "       }\n"
        "      \n"
        "       z = scale * z - offset * (scale - 1.0f);\n"
        "       \n"
        "       orbit = max(orbit, z * m);\n"
        "   }\n"
        "\n"
        "   return orbit;\n"
        "\n"
        "}\n";

SierpinskiTriangle::SierpinskiTriangle()
        : Fractal(distance_function_code, orbit_function_code) {}