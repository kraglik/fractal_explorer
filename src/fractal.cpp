#include "fractal.h"
#include <fstream>
#include <exception>


Fractal::Fractal(const std::string & distance_function_code, const std::string & orbit_function_code) {

    std::ifstream kernel_part_1_file("kernels/kernel_part_1.cl");
    std::ifstream kernel_part_2_file("kernels/kernel_part_2.cl");

    if (!kernel_part_1_file.is_open() || !kernel_part_2_file.is_open()) {
        throw * new std::runtime_error("Kernel file was not found.");
    }

    std::string & kernel_part_1 = * new std::string(
        std::istreambuf_iterator<char>(kernel_part_1_file),
        std::istreambuf_iterator<char>()
    );

    std::string & kernel_part_2 = * new std::string(
            std::istreambuf_iterator<char>(kernel_part_2_file),
            std::istreambuf_iterator<char>()
    );

    kernel = new std::string();
    *kernel += kernel_part_1;
    *kernel += distance_function_code;
    *kernel += orbit_function_code;
    *kernel += kernel_part_2;

}

Camera Fractal::get_initial_camera() {
    return Camera({-10, 0, 0}, {1, 0, 0}, {0, 1, 0});
}
