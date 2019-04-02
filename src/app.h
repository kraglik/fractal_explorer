#ifndef FRACTAL_EXPLORER_APP_H
#define FRACTAL_EXPLORER_APP_H

#include "cl_context.h"
#include "camera.h"
#include "render.h"
#include "fractal.h"
#include <vector>

class App {

    public:
        App(int width, int height, bool centered, bool fullscreen);

        void run();

    private:
        std::vector<Fractal &> fractals;
        CLContext & context;
        Camera & camera;
        Render & render;

};


#endif