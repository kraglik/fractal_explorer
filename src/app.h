#ifndef FRACTAL_EXPLORER_APP_H
#define FRACTAL_EXPLORER_APP_H

#include "cl_context.h"
#include "camera.h"
#include "render.h"
#include "fractal.h"
#include <vector>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <GL/gl.h>

class App {

    public:
        App(int width, int height, bool centered);

        void run();

    private:
        void toggle_fullscreen();
        void render(Render & renderer);

        std::vector<Render> & renderers = *new std::vector<Render>();
        CLContext context;
        SDL_Window * window;
        SDL_GLContext gl_context;
        uint32_t window_flags;
        bool fullscreen;
        int win_height, win_width, screen_width, screen_height;
        GLuint fullscreen_texture_id, texture_id;
        cl::Image2D fullscreen_image, image;
};


#endif