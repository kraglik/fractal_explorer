#include "fractal.h"
#include <fstream>
#include <exception>


Fractal::Fractal(const char *path_to_code) {

    std::ifstream kernel_file(path_to_code);

    if (!kernel_file.is_open()) {
        throw * new std::runtime_error("Kernel file was not found.");
    }

    kernel = new std::string(
        std::istreambuf_iterator<char>(kernel_file),
        std::istreambuf_iterator<char>()
    );

}

Camera Fractal::get_initial_camera() {
    return Camera({-10, 0, 0}, {1, 0, 0}, {0, 1, 0});
}
