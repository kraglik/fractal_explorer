#include "app.h"
#include <SDL2/SDL.h>
#include <iostream>
#include <exception>

#pragma OPENCL EXTENSION CL_APPLE_gl_sharing : enable
#pragma OPENCL EXTENSION CL_KHR_gl_sharing : enable

App::App(int width, int height, bool centered) {
    this->fullscreen = false;

    win_height = height;
    win_width = width;

    window_flags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN;

    if (SDL_Init(SDL_INIT_EVERYTHING) != 0){
        std::cerr << "SDL_Init error: " << SDL_GetError() << std::endl;
        throw * new std::runtime_error("Failed to initialize sdl2");
    }

    window = SDL_CreateWindow("Fractal Explorer", 100, 100, win_width, win_height, window_flags);
    SDL_HideWindow(window);

    if (window == nullptr){
        std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        throw * new std::runtime_error("Failed to initialize window");
    }

    gl_context = SDL_GL_CreateContext(window);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

    context = CLContext(gl_context);

    SDL_DisplayMode DM;
    SDL_GetCurrentDisplayMode(0, &DM);

    screen_height = DM.h;
    screen_width = DM.w;

    // Initializing OpenGL textures
    glGenTextures(1, &fullscreen_texture_id);
    glBindTexture(GL_TEXTURE_2D, fullscreen_texture_id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, screen_width, screen_height, 0, GL_RGBA, GL_FLOAT, nullptr);

    glGenTextures(1, &texture_id);
    glBindTexture(GL_TEXTURE_2D, texture_id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, win_width, win_height, 0, GL_RGBA, GL_FLOAT, nullptr);

    // Initializing OpenCL Buffers
    cl_int texture_error;
    cl::Context & cl_context = context.get_context();
    image = clCreateFromGLTexture(
        cl_context(),
        CL_MEM_WRITE_ONLY,
        GL_TEXTURE_2D,
        0,
        texture_id,
        &texture_error
    );

    if (texture_error)
        throw * new std::runtime_error("Failed to initialize CL Image2D buffer.");

    fullscreen_image = clCreateFromGLTexture(
        cl_context(),
        CL_MEM_WRITE_ONLY,
        GL_TEXTURE_2D,
        0,
        fullscreen_texture_id,
        &texture_error
    );

    if (texture_error)
        throw * new std::runtime_error("Failed to initialize CL Image2D buffer.");

    // Initializing fractal renderers
    cl_float3 sun_direction = {-1, 1, -1};

    Fractal & mandelbox = * new Fractal("kernels/kernel.cl");
    renderers.push_back(* new Render(context, sun_direction, mandelbox));

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
    glDisable(GL_LIGHTING);
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
}

void App::run() {
    SDL_ShowWindow(window);

    glViewport(0, 0, win_width, win_height);

    bool running = true;
    cl_float3 movement = {0, 0, 0};
    uint32_t renderer_index = 0;

    while (running) {
        SDL_Event event;
        Render & renderer = renderers[renderer_index];

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT)
                running = false;

            if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                    case SDLK_F11:
                        fullscreen = !fullscreen;
                        toggle_fullscreen();
                        glViewport(0, 0,
                            fullscreen ? screen_width : win_width,
                            fullscreen ? screen_height : win_height
                        );
                        break;

                    case SDLK_ESCAPE:
                        running = false;
                        break;

                    default:
                        break;
                }
            }

            if (event.type == SDL_KEYUP) {
                switch (event.key.keysym.sym) {
                    default:
                        break;
                }
            }
        }

        render(renderer);
    }

    SDL_DestroyWindow(window);
}

void App::toggle_fullscreen() {
    if (fullscreen) {
        if (SDL_SetWindowFullscreen(window, window_flags | SDL_WINDOW_FULLSCREEN_DESKTOP))
            std::cerr << "Fullscreen mode error" << std::endl;
        else
            SDL_SetWindowSize(window, screen_width, screen_height);
    }
    else {
        SDL_SetWindowFullscreen(window, window_flags);
        SDL_SetWindowSize(window, win_width, win_height);
    }
}

void App::render(Render & renderer) {

    int width = fullscreen ? screen_width : win_width;
    int height = fullscreen ? screen_height : win_height;

    cl::Image2D & current_buffer = fullscreen ? fullscreen_image : image;

    glFinish();
    renderer.render(current_buffer, width, height);

    glLoadIdentity();

    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT);

    glEnable(GL_TEXTURE_2D);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, fullscreen ? fullscreen_texture_id : texture_id);

    glBegin(GL_QUADS);

    glTexCoord2f(0, 1); glVertex2f(-1, -1);
    glTexCoord2f(1, 1); glVertex2f(1, -1);
    glTexCoord2f(1, 0); glVertex2f(1, 1);
    glTexCoord2f(0, 0); glVertex2f(-1, 1);

    glEnd();

    glDisable(GL_TEXTURE_2D);

    glFlush();

    SDL_GL_SwapWindow(window);
}
