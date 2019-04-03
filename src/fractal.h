#ifndef FRACTAL_EXPLORER_FRACTAL_H
#define FRACTAL_EXPLORER_FRACTAL_H

#include <string>
#include "cl_context.h"
#include "camera.h"

class Fractal {

public:
    Fractal(const char * path_to_code);

    std::string & get_kernel() { return *kernel; };

    Camera get_initial_camera();

private:
    std::string * kernel;
};


#endif