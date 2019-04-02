#ifndef FRACTAL_EXPLORER_FRACTAL_H
#define FRACTAL_EXPLORER_FRACTAL_H

#include <string>
#include "cl_context.h"

class Fractal {

public:
    typedef struct {

    } FractalCL;

    Fractal(CLContext & context,
            const char * path_to_de,
            const char * path_to_orbit_trap,);

    const std::string & get_kernel() const;

private:
    std::string & kernel;
    CLContext & context;
    cl::Program program;
};


#endif